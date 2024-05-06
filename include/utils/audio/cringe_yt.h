#ifndef CRINGE_CRINGE_YT_H
#define CRINGE_CRINGE_YT_H

#include <string>
#include <dpp/dpp.h>
#include <algorithm>
#include <regex>
#include <fmt/format.h>
#include "utils/audio/cringe_song_new.h"
#include "sstream"

class CringeYoutube {
	Song get_content(const std::string &query);
	static std::string to_url(const std::string &query);
	static bool is_url(const std::string &query);
	static bool is_playlist(const std::string &query);
	static std::string search_command(std::string search);
private:
	static std::string sanitize(std::string query);
};

#endif
