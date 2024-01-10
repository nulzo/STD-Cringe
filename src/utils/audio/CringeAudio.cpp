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

#include "utils/cringe.h"
#include <algorithm>
#include <regex>
#include <fmt/format.h>
#include "sstream"

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswresample/swresample.h>
#include <opus/opus.h>
}

bool Cringe::CringeAudio::is_url(const std::string &query) {
	// Regular expression to determine if a given query is a URL
	std::regex url_regex(R"(^(https?|ftp):\/\/[^\s/$.?#].[^\s]*)");
	// Match the user query against the regular expression definition
	return std::regex_match(query, url_regex);
}

std::vector<std::string> Cringe::CringeAudio::get_yt_info(std::string song) {
	// Allocate c style buf to store result of command
	char buffer[128];
	std::string result;
	std::vector<std::string> yt_data;
	std::string data;
	// string that gets all the information about the song
	std::string cmd = fmt::format("yt-dlp --print title --print uploader --print thumbnail --print duration \"{}\"", song);
	// Get the YouTube video thumbnail, author info, and song info
	FILE *pipe = popen(cmd.c_str(), "r");
	// Check that the pipe was opened successfully
	if (!pipe) {
		std::cerr << "Error opening pipe" << std::endl;
	}
	// Write contents of stdout buf to c++ style string
	while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
		result += buffer;
	}
	std::istringstream stream(result);
	// Loop through line and add to vector
	while (std::getline(stream, data, '\n')) {
		yt_data.push_back(data);
	}
	// Returns a vector with the elements <TITLE, ARTIST, THUMB_URL, DURATION>
	return yt_data;
}

std::string Cringe::CringeAudio::search_command(std::string search) {
	return fmt::format(R"(yt-dlp -f bestaudio -o - "{}" | ffmpeg -i pipe:0 -loglevel warning -f s16le -ac 2 -ar 48000 pipe:1)", search);
}

std::string Cringe::CringeAudio::query_to_url(std::string query) {
	// Allocate c style buf to store result of command
	char buffer[128];
	// Var to store processed URL
	std::string URL;
	// The query to get the YouTube URL
	std::string command = fmt::format("yt-dlp --print webpage_url \"ytsearch:{}\"", query);
	// Convert to a C-string
	const char *cmd = command.c_str();
	// Open the pipe to process to command
	FILE *pipe = popen(cmd, "r");
	// Write contents of stdout buf to c++ style string
	while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
		URL += buffer;
	}
	// Return the URL
	return URL;
}

void Cringe::CringeAudio::cringe_streamer(const std::string &ffmpeg_data, dpp::voiceconn *voice) {
	// We need to set this otherwise the bot attempts to send all packets at once
	// Note: This is expensive!!! Might need to figure out another way...
	voice->voiceclient->set_send_audio_type(dpp::discord_voice_client::satype_overlap_audio);
	// buf to store contents
	std::byte buf[11520];
	// Subprocess fork that runs our ffmpeg piped audio
	auto pipe = popen(ffmpeg_data.c_str(), "r");
	// Bytes from output
	size_t bytes_read;
	// Get audio from song and pipe to discord
	while ((bytes_read = fread(buf, sizeof(std::byte), dpp::send_audio_raw_max_length, pipe)) > 0) {
		if (bytes_read >= dpp::send_audio_raw_max_length) {
			// Send audio data if cringe is in an on-ready state
			if (voice->voiceclient && voice->voiceclient->is_ready()) {
				voice->voiceclient->send_audio_raw((uint16_t *) buf, sizeof(buf));
			}
		}
	}
	voice->voiceclient->insert_marker();
	pclose(pipe);
}

std::string Cringe::CringeAudio::sanitize_query(std::string query) {
	query.erase(std::remove(query.begin(), query.end(), '\n'), query.end());
	query.erase(std::remove(query.begin(), query.end(), '"'), query.end());
	query.erase(std::remove(query.begin(), query.end(), '{'), query.end());
	query.erase(std::remove(query.begin(), query.end(), '}'), query.end());
	return query;
}
