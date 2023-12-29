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

void play_callback(dpp::cluster &bot, Cringe::CringeSong song) {
	dpp::slashcommand_t event = song.get_event();
	// Get the voice channel the bot is in, in this current guild.
	dpp::voiceconn *voice = event.from->get_voice(event.command.guild_id);

	// If the voice channel was invalid, or there is an issue with it, then tell the user.
	if (!voice || !voice->voiceclient || !voice->voiceclient->is_ready()) {
		std::cerr << "Bot was unable to join the voice channel due to some unknown reason.";
		return;
	}

	// Set the url and codec, piping the audio with ffmpeg
	std::string song_streamer = Cringe::CringeAudio::search_command(song.get_url());

	// Convert to a c string
	const char *audio_codec = song_streamer.c_str();

	// buf to store contents
	std::byte buf[11520];

	// Subprocess fork that runs our ffmpeg piped audio
	auto pipe = popen(audio_codec, "r");

	// Send an embed letting user know that the bot is playing
	dpp::message message(event.command.channel_id, now_streaming(song));
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

	// Remove newline characters from the request
	song = Cringe::CringeAudio::sanitize_query(song);

	// Determine if the song is not a URL. Currently, we can only get URLs, but we will add the option for queries in the future.
	if (!Cringe::CringeAudio::is_yt_url(song)) {
		// Convert the query into the corresponding URL
		song = Cringe::CringeAudio::query_to_url(song);
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
	dpp::message message(event.command.channel_id, now_streaming(s));
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