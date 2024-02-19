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

//  ====================  AUXILIARY FUNCTIONS ====================

void play_callback(dpp::cluster &bot, Cringe::CringeSong song) {
	auto voice = song.get_event().from->get_voice(song.get_event().command.guild_id);
	// Set the url and codec, piping the audio with ffmpeg
	std::string process = Cringe::CringeAudio::search_command(song.get_url());
	// Add filter (if applicable)
	if (!song.get_filter().empty()) process += fmt::format(" -vn -filter_complex {}", song.get_filter());
	// Send in proper channel
	dpp::message message(song.get_event().command.channel_id, now_streaming(song));
	bot.message_create(message);
	Cringe::CringeAudio::cringe_streamer(process, voice);
}

void playlist_streamer(std::string song, Cringe::CringeQueue &queue, const dpp::slashcommand_t &event, dpp::voiceconn *voice, dpp::cluster &bot) {
	// If it is a playlist, we can add all the songs to the queue
	std::string subprocess = fmt::format("youtube-dl -j --flat-playlist '{}' | jq -r '.id' | sed 's_^_https://youtube.com/v/_'", song);
	const char *subp = subprocess.c_str();
	std::string result;
	// Read the output of the command into a string
	char buffer[4096];
	// Open a pipe to run the command
	FILE *pipe = popen(subp, "r");
	if (!pipe) {
		std::cerr << "Error opening pipe to youtube-dl" << std::endl;
	}
	while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
		result += buffer;
	}
	// Close the pipe
	pclose(pipe);
	// Vector to store the split substrings
	std::vector<std::string> songs;
	// Use std::istringstream and std::getline to split the string
	std::istringstream playlist(result);
	std::string playlist_entry;
	// Loop through songs and add to queue
	while (std::getline(playlist, playlist_entry)) {
		std::cout << "\n\nLOOPING\n\n";
		// Get the YouTube upload info from the url given
		std::vector<std::string> yt_info = Cringe::CringeAudio::get_yt_info(playlist_entry);
		// Make a new song to store to the queue
//		Cringe::CringeSong s(yt_info[0], yt_info[1], yt_info[2], yt_info[3], playlist_entry, (dpp::slashcommand_t &) event);
		// Add song to the queue
//		queue.enqueue(s);
	}
}

//  =================  END AUXILIARY FUNCTIONS ===================


dpp::slashcommand play_declaration() {
	return dpp::slashcommand()
			.set_name("play")
			.set_description("Play a song in the voice channel you are in")
			.add_option(dpp::command_option(dpp::co_string, "song", "Song you wish to stream (either query or URL)", true))
			.add_option(dpp::command_option(dpp::co_string, "filter", "Filter to apply to the song", false)
								.add_choice(dpp::command_option_choice("Bass Boosted", std::string("bassboost")))
								.add_choice(dpp::command_option_choice("Vaporwave", std::string("vaporwave")))
								.add_choice(dpp::command_option_choice("Nightcore", std::string("nightcore")))
								.add_choice(dpp::command_option_choice("In The Bathroom", std::string("bathroom")))
								.add_choice(dpp::command_option_choice("Lofi", std::string("lofi")))
								.add_choice(dpp::command_option_choice("DIM", std::string("dim")))
								.add_choice(dpp::command_option_choice("Expander", std::string("expand"))));
}

void play_command(dpp::cluster &bot, const dpp::slashcommand_t &event, Cringe::CringeQueue &queue) {
	// Set the bot to thinking. This gives us a bit more time to reply to the interaction
	event.thinking(true);
	std::string channel;
	get_env("CRINGE_MUSIC_CHANNEL", channel);
	// Get the voice channel the bot is in, in this current guild.
	dpp::voiceconn *voice = event.from->get_voice(event.command.guild_id);
	// If the voice channel was invalid, or there is an issue with it, then tell the user.
	if (!voice || !voice->voiceclient || !voice->voiceclient->is_ready()) {
		dpp::message message(event.command.channel_id, status_embed("CringeError::VoiceError", "Bot was unable to join the voice channel due to some unknown reason.", Cringe::CringeStatus::ERROR));
		event.edit_original_response(message);
		return;
	}
	// Get the song that the user wishes to play
	std::string request = std::get<std::string>(event.get_parameter("song"));
	// Get a filter (if one was given)
	auto check = event.get_parameter("filter");
	std::string filter;
	if (check.index() > 0) {
		Cringe::CringeFilter cringe_filter;
		std::string parameter = std::get<std::string>(check);
		if (parameter == "bassboost") filter = cringe_filter.BASSBOOST;
		if (parameter == "vaporwave") filter = cringe_filter.VAPORWAVE;
		if (parameter == "nightcore") filter = cringe_filter.NIGHTCORE;
		if (parameter == "bathroom") filter = cringe_filter.INTHEBATHROOM;
		if (parameter == "lofi") filter = cringe_filter.LOFI;
		if (parameter == "dim") filter = cringe_filter.DIM;
		if (parameter == "expand") filter = cringe_filter.EXPANDER;
	}
	// Remove newline characters from the request
	request = Cringe::CringeAudio::sanitize_query(request);
	// Determine if the song is not a URL. Currently, we can only get URLs, but we will add the option for queries in the future.
	if (!Cringe::CringeAudio::is_url(request)) {
		// Convert the query into the corresponding URL
		request = Cringe::CringeAudio::query_to_url(request);
	}
	// Define the regular expression pattern to see if this is a YouTube playlist
	std::regex pattern("&(start_radio=|list=)");
	// Check if it is a YouTube playlist
	if (std::regex_search(request, pattern)) {
		// Reply to the event letting the issuer know the song was queued
		dpp::message message(event.command.channel_id, "You cannot queue a playlist at this time!");
		event.edit_original_response(message);
		return;
	}
	// Check if queue is not empty (or if song is currently playing)
	if (!queue.is_empty() || voice->voiceclient->is_playing()) {
		// Get the YouTube upload info from the url given
		std::vector<std::string> yt_info = Cringe::CringeAudio::get_yt_info(request);
		// Make a new song to store to the queue
		Cringe::CringeSong song(yt_info[0], yt_info[1], yt_info[2], yt_info[3], yt_info[4], yt_info[5], yt_info[6], yt_info[7], filter, request, (dpp::slashcommand_t &) event);
		// Add song to the queue
		queue.enqueue(song);
		// Reply to the event letting the issuer know the song was queued
		dpp::message message(event.command.channel_id, added_to_queue_embed(song));
		event.edit_original_response(message);
		return;
	}
	// Set the url and codec, piping the audio with ffmpeg
	std::string process = Cringe::CringeAudio::search_command(request);
	// Add filter (if applicable)
	if (!filter.empty()) process += fmt::format(" -vn -filter_complex {}", filter);
	// Get the song information from the command
	std::vector<std::string> yt_info = Cringe::CringeAudio::get_yt_info(request);
	// Create a new song object and populate it with our new information
	Cringe::CringeSong song(yt_info[0], yt_info[1], yt_info[2], yt_info[3], yt_info[4], yt_info[5], yt_info[6], yt_info[7], filter, request, (dpp::slashcommand_t &) event);
	// Send in proper channel
	dpp::message message(channel, now_streaming(song));
	// Send the embed
	bot.message_create(message);
	// Ephemeral embed saying that the command is playing. All events must be responded to
	dpp::message msg(event.command.channel_id, added_to_queue_embed(song));
	event.edit_original_response(msg);
	Cringe::CringeAudio::cringe_streamer(process, voice);
}