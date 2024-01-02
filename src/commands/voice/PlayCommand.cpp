/*
 * MIT License
 *
 * Copyright (c) 2023 @nulzo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "commands/voice/PlayCommand.h"
#include "utils/embed.h"
#include "utils/cringe.h"
#include <algorithm>
#include <fmt/format.h>
#include <regex>
#include "dpp/dpp.h"
#include "utils/util.h"
#include <sstream>

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
			.add_option(dpp::command_option(dpp::co_string, "song", "Song you wish to stream (either query or URL)", true))
			.add_option(dpp::command_option(dpp::co_string, "filter", "Filter to apply to the song", false)
								.add_choice(dpp::command_option_choice("Bass Boosted", std::string("bassboost")))
								.add_choice(dpp::command_option_choice("Vaporwave", std::string("vaporwave")))
								.add_choice(dpp::command_option_choice("Nightcore", std::string("nightcore")))
								.add_choice(dpp::command_option_choice("In The Bathroom", std::string("bathroom"))));
}

void play_callback(dpp::cluster &bot, Cringe::CringeSong song) {
	std::string filter;
	Cringe::CringeFilter cringe_filter;
	std::string parameter;
	dpp::slashcommand_t event = song.get_event();

	// Get the voice channel the bot is in, in this current guild.
	dpp::voiceconn *voice = event.from->get_voice(event.command.guild_id);

	// If the voice channel was invalid, or there is an issue with it, then tell the user.
	if (!voice || !voice->voiceclient || !voice->voiceclient->is_ready()) {
		std::cerr << "Bot was unable to join the voice channel due to some unknown reason.";
		return;
	}

	// Get a filter (if one was given)
	auto check = event.get_parameter("filter");
	if(check.index() > 0) {
		parameter = std::get<std::string>(check);
		if (parameter == "bassboost") filter = cringe_filter.BASSBOOST;
		if (parameter == "vaporwave") filter = cringe_filter.VAPORWAVE;
		if (parameter == "nightcore") filter = cringe_filter.NIGHTCORE;
		if (parameter == "bathroom") filter = cringe_filter.INTHEBATHROOM;
	}

	// Set the url and codec, piping the audio with ffmpeg
	std::string song_streamer = Cringe::CringeAudio::search_command(song.get_url());

	// Add filter (if applicable)
	if (!filter.empty()) song_streamer += fmt::format(" -vn -filter_complex {}", filter);

	// Convert to a c string
	const char *audio_codec = song_streamer.c_str();

	// buf to store contents
	std::byte buf[11520];

	// Subprocess fork that runs our ffmpeg piped audio
	auto pipe = popen(audio_codec, "r");

	// Send an embed letting user know that the bot is playing
	dpp::message message(1081850403920035931, now_streaming(song));
	bot.message_create(message);

	// Bytes from output
	size_t bytes_read;

	// Get audio from song and pipe to discord
	while ((bytes_read = fread(buf, sizeof(std::byte), 11520, pipe)) > 0) {
		if (bytes_read < dpp::send_audio_raw_max_length) {
			continue;
		}
		// Send audio data if cringe is in an on-ready state
		if (voice->voiceclient && voice->voiceclient->is_ready()) {
			voice->voiceclient->send_audio_raw((uint16_t *) buf, sizeof(buf));
		}
	}
	voice->voiceclient->insert_marker();
	pclose(pipe);
}

void play_command(dpp::cluster &bot, const dpp::slashcommand_t &event, Cringe::CringeQueue &queue) {
	std::string embed_reason;
	dpp::embed embed;
	dpp::channel requested_channel;
	dpp::voiceconn *voice;
	std::string filter;
	Cringe::CringeFilter cringe_filter;
	std::string parameter;

	// Set the bot to thinking. This gives us a bit more time to reply to the interaction
	event.thinking(true);

	/* Get the voice channel the bot is in, in this current guild. */
	voice = event.from->get_voice(event.command.guild_id);

	/* If the voice channel was invalid, or there is an issue with it, then tell the user. */
	if (!voice || !voice->voiceclient || !voice->voiceclient->is_ready()) {
		dpp::message message(event.command.channel_id, status_embed("CringeError::VoiceError", "Bot was unable to join the voice channel due to some unknown reason.", Cringe::CringeStatus::ERROR));
		event.edit_original_response(message);
		return;
	}

	// Get the song that the user wishes to play
	std::string song = std::get<std::string>(event.get_parameter("song"));

	// Get a filter (if one was given)
	auto check = event.get_parameter("filter");
	if(check.index() > 0) {
		parameter = std::get<std::string>(check);
		if (parameter == "bassboost") filter = cringe_filter.BASSBOOST;
		if (parameter == "vaporwave") filter = cringe_filter.VAPORWAVE;
		if (parameter == "nightcore") filter = cringe_filter.NIGHTCORE;
		if (parameter == "bathroom") filter = cringe_filter.INTHEBATHROOM;
	}

	// Remove newline characters from the request
	song = Cringe::CringeAudio::sanitize_query(song);

	// Determine if the song is not a URL. Currently, we can only get URLs, but we will add the option for queries in the future.
	if (!Cringe::CringeAudio::is_url(song)) {
		// Convert the query into the corresponding URL
		song = Cringe::CringeAudio::query_to_url(song);
	}

	// Define the regular expression pattern to see if this is a youtube playlist
	std::regex pattern("&(start_radio=|list=)");

	// Check if it is a YouTube playlist
	if (std::regex_search(song, pattern)) {
		std::cout << "\n\nPLAYLIST\n\n";
		// If it is a playlist, we can add all the songs to the queue
		std::string subprocess = fmt::format("youtube-dl -j --flat-playlist '{}' | jq -r '.id' | sed 's_^_https://youtube.com/v/_'", song);
		const char * subp = subprocess.c_str();
		std::string result;
		// Read the output of the command into a string
		char buffer[1024];
		// Open a pipe to run the command
		FILE* pipe = popen(subp, "r");
		if (!pipe) {
			std::cerr << "Error opening pipe to youtube-dl" << std::endl;
			return;
		}
		while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
			result += buffer;
		}
		// Close the pipe
		pclose(pipe);
		std::cout << "\n\nCLOSED PIPe\n\n" << result << "\n\n";

		// Vector to store the split substrings
		std::vector<std::string> songs;

		// Use std::istringstream and std::getline to split the string
		std::istringstream playlist(result);
		std::string playlist_entry;

		while (std::getline(playlist, playlist_entry)) {
			std::cout << "\n\nLOOPING\n\n" << playlist_entry << "\n\n";
			songs.push_back(playlist_entry);
		}

		// Display the elements in the vector
		for (const auto& elsong : songs) {
			// Get the YouTube upload info from the url given
			std::vector<std::string> yt_info = Cringe::CringeAudio::get_yt_info(elsong);
			// Make a new song to store to the queue
			Cringe::CringeSong s(yt_info[0], yt_info[1], yt_info[2], yt_info[3], song, (dpp::slashcommand_t &) event);
			// Add song to the queue
			queue.enqueue(s);
			std::cout << "\n\nQUEUEING\n\n" << s.get_title() << "\n\n";
		}
		// Reply to the event letting the issuer know the song was queued
		dpp::message message(event.command.channel_id, "Your playlist was added!");
		event.edit_original_response(message);
		return;
	}

	// Check if queue is not empty (or if song is currently playing)
	if (!queue.is_empty() || voice->voiceclient->is_playing()) {
		// Get the YouTube upload info from the url given
		std::vector<std::string> yt_info = Cringe::CringeAudio::get_yt_info(song);
		// Make a new song to store to the queue
		Cringe::CringeSong s(yt_info[0], yt_info[1], yt_info[2], yt_info[3], song, (dpp::slashcommand_t &) event);
		// Add song to the queue
		queue.enqueue(s);
		// Reply to the event letting the issuer know the song was queued
		dpp::message message(event.command.channel_id, added_to_queue_embed(s));
		event.edit_original_response(message);
		return;
	}

	// Set the url and codec, piping the audio with ffmpeg
	std::string song_streamer = Cringe::CringeAudio::search_command(song);
	// Add filter (if applicable)
	if (!filter.empty()) song_streamer += fmt::format(" -vn -filter_complex {}", filter);
	// Cast to c string for popen process
	const char *audio_codec = song_streamer.c_str();
	// buf to store contents
	std::byte buf[11520];
	// Subprocess fork that runs our ffmpeg piped audio
	auto pipe = popen(audio_codec, "r");
	// Check if pipe was unsuccessful
	if (!pipe) {
		std::cout << "Failed to open Pipe" << std::endl;
		return;
	}
	// Get the song information from the command
	std::vector<std::string> yt_info = Cringe::CringeAudio::get_yt_info(song);
	// Create a new song object and populate it with our new information
	Cringe::CringeSong s(yt_info[0], yt_info[1], yt_info[2], yt_info[3], song, (dpp::slashcommand_t &) event);
	// Embed letting user know that the bot is playing
	dpp::message message(1081850403920035931, now_streaming(s));
	// Send the embed
	bot.message_create(message);
	// Ephemeral embed saying that the command is playing. All events must be responded to
	dpp::message msg(event.command.channel_id, added_to_queue_embed(s));
	event.edit_original_response(msg);

	// We need to set this otherwise the bot attempts to send all packets at once
	// Note: This is expensive!!! Might need to figure out another way...
	voice->voiceclient->set_send_audio_type(dpp::discord_voice_client::satype_overlap_audio);

	// Bytes from output
	size_t bytes_read;

	// Get audio from song and pipe to discord
	while ((bytes_read = fread(buf, sizeof(std::byte), 11520, pipe)) > 0) {
		if (bytes_read < 11520) {
			std::cout << "11520 was bigger than bytes read: " << bytes_read << std::endl;
			continue;
		}
		// Send audio data if cringe is in an on-ready state
		if (voice->voiceclient && voice->voiceclient->is_ready()) {
			voice->voiceclient->send_audio_raw((uint16_t *) buf, sizeof(buf));
		} else {
			std::cout << "VoiceClient was not ready" << std::endl;
		}
	}
	voice->voiceclient->insert_marker();
	std::cout << "Reached the end of stream" << std::endl;
	pclose(pipe);
}