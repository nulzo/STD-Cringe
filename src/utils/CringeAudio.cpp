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

bool Cringe::CringeAudio::is_yt_url(const std::string &query) {
	// Regular expression to determine if a given url is from youtube
	std::regex youtube_regex(
			R"(^(?:(?:https?:)?\/\/)?(?:www\.)?(?:youtube\.com\/(?:[^\/\n\s]+\/\S+\/|(?:v|e(?:mbed)?)\/|\S*?[?&]v=)|youtu\.be\/)([a-zA-Z0-9_-]{11}))");
	// Match the user query against the regular expression definition
	bool is_youtube_link = std::regex_match(query, youtube_regex);
	return (is_youtube_link);
}

std::vector<std::string> Cringe::CringeAudio::get_yt_info(std::string song) {
	// Allocate c style buf to store result of command
	char buffer[128];
	std::string result;
	std::vector<std::string> yt_data;
	std::string data;
	// string that gets all the information about the song
	std::string cmd = fmt::format("yt-dlp --print artist --print title --print upload_date --print duration --print thumbnail \"{}\"", song);
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

	// Returns a vector with the elements <ARTIST, TITLE, UPLOAD_DATE, DURATION, THUMB_URL>
	return yt_data;
}

std::string Cringe::CringeAudio::get_encoded_url(std::string song){
	// Allocate c style buf to store result of command
	char buffer[128];
	// Var to store processed URL
	std::string URL;
	// Get the command to extract the URL
	std::string command = fmt::format("yt-dlp --get-url -f bestaudio \"{}\"", song);
	// Convert to a C-string
	const char * cmd = command.c_str();
	// Open the pipe to process to command
	FILE *pipe = popen(cmd, "r");
	// Check that the pipe was opened successfully
	if (!pipe) {
		std::cerr << "Error opening pipe" << std::endl;
	}
	// Write contents of stdout buf to c++ style string
	while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
		URL += buffer;
	}
	// Return the URL
	return URL;
}
