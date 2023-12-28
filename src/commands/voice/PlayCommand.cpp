#include "commands/voice.h"
#include "utils/embed.h"
#include "utils/cringe.h"
#include <algorithm>
#include <regex>
#include <fmt/format.h>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswresample/swresample.h>
#include <opus/opus.h>
}

dpp::slashcommand play_declaration() {
	return dpp::slashcommand()
			.set_name("play")
			.set_description("Play a song in the voice channel you are in")
			.add_option(dpp::command_option(dpp::co_string, "song", "Song you wish to stream (either query or URL)", true));
}

void play_command(const dpp::cluster &bot, const dpp::slashcommand_t &event, Cringe::CringeQueue& queue) {

	std::string embed_reason;
	dpp::embed embed;
	dpp::guild *guild;
	dpp::snowflake parameter;
	dpp::channel requested_channel;
	Cringe::CringeAudio extractor;

	dpp::guild *g = dpp::find_guild(event.command.guild_id);

	/* Get the voice channel the bot is in, in this current guild. */
	dpp::voiceconn *v = event.from->get_voice(event.command.guild_id);

	/* If the voice channel was invalid, or there is an issue with it, then tell the user. */
	if (!v || !v->voiceclient || !v->voiceclient->is_ready()) {
		dpp::message message(event.command.channel_id, status_embed("CringeError::VoiceError", "Bot was unable to join the voice channel due to some unknown reason.", Cringe::CringeStatus::ERROR));
		event.reply(message);
		return;
	}

	// We need to set this otherwise the bot attempts to send all packets at once
	// Note: This is expensive!!! Might need to figure out another way...
	v->voiceclient->set_send_audio_type(dpp::discord_voice_client::satype_overlap_audio);

	// Get the song that the user wishes to play
	std::string song = std::get<std::string>(event.get_parameter("song"));

	// Set the bot to thinking. This gives us a bit more time to reply to the interaction
	event.thinking();

	// Remove newline characters from the request
	song.erase(std::remove(song.begin(), song.end(), '\n'), song.end());

	// First determine if the song is not a URL. Currently, we can only get URLs, but we will add the option for queries in the future.
	if (!Cringe::CringeAudio::is_yt_url(song)) {
		// Issue an error if the query is not a URL
		embed_reason = "cringe can only parse through URL's to youtube at the moment :) Give it some time!";
		embed = status_embed("CringeError::VoiceError", embed_reason, Cringe::CringeStatus::ERROR);
		dpp::message message(event.command.channel_id, embed);
		event.edit_original_response(message);
		return;
	}

	// Check if queue is empty, add if not
	if(!queue.is_empty()) {
		// At this point, there are songs to be played ahead of the current song.
		// Get URL and save info for later
		std::string url = Cringe::CringeAudio::get_encoded_url(song);
		// Get the youtube upload info from the url given
		std::vector<std::string> yt_info = Cringe::CringeAudio::get_yt_info(song);
		// Make a new song to store to the queue
		Cringe::CringeSong s(yt_info[0], yt_info[1], yt_info[2], yt_info[3]);
		// Add song to the queue
		queue.enqueue(s);
		// Reply to the event letting the issuer know the song was queued
		dpp::message message(event.command.channel_id, "Hermph");
		event.reply(message);
	}

	// Set the url and codec, piping the audio with ffmpeg
	std::string song_streamer = "yt-dlp -f bestaudio -q --ignore-errors -o - \"" + song +
								"\" | ffmpeg -i pipe:0 -f s16le -ac 2 -ar 48000 pipe:1";
	// -loglevel quiet
	const char *audio_codec = song_streamer.c_str();

	// buf to store contents
	std::byte buf[11520];

	// string that gets all the information about the song
	std::string cmd = "yt-dlp --print artist --print title --print upload_date --print duration --print thumbnail \"" + song + "\"";

	// Extract information for the embed
	std::string output = get_youtube_elements(cmd.c_str());

	// Print the captured output
	std::cout << "Command Output:\n" << output << std::endl;

	// Split the output into lines
	std::vector<std::string> lines = split_string(output, '\n');

	// Process each line
	for (const auto &line: lines) {
		std::cout << "Element: " << line << std::endl;
	}

	std::cout << audio_codec << std::endl;

	// Subprocess fork that runs our ffmpeg piped audio
	auto pipe = popen(audio_codec, "r");

	// Check if pipe was unsuccessful
	if (!pipe) {
		std::cout << "Failed to open Pipe" << std::endl;
		return;
	}

	// Embed letting user know that the bot is playing
	embed = playing_embed(Cringe::CringeStatus::SUCCESS);
	dpp::message message(event.command.channel_id, embed);
	event.edit_original_response(message);

	// Bytes from output
	size_t bytes_read;

	// Get audio from song and pipe to discord
	while ((bytes_read = fread(buf, sizeof(std::byte), 11520, pipe)) > 0) {
		if (bytes_read < 11520) {
			std::cout << "11520 was bigger than bytes read: " << bytes_read << std::endl;
			continue;
		}
		// Send audio data if cringe is in an on-ready state
		if (v && v->voiceclient && v->voiceclient->is_ready()) {
			v->voiceclient->send_audio_raw((uint16_t *) buf, sizeof(buf));
		} else {
			std::cout << "VoiceClient was not ready" << std::endl;
		}
	}
	std::cout << "Reached the end of stream" << std::endl;
	pclose(pipe);
}