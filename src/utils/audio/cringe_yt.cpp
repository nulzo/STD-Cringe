#include "utils/audio/cringe_youtube.h"

CringeSong CringeYoutube::get_content(const std::string &query) {
	std::string sanitized = this->sanitize(query);
	if(!this->is_url(sanitized)) {
		sanitized = to_url(sanitized);
	}
    CringeSong song;
    char buffer[128];
    std::string result;
    std::string cmd = fmt::format("yt-dlp -s --print title --print channel --print thumbnail --print duration_string --print view_count --print comment_count --print release_timestamp --print channel_follower_count --print description --print webpage_url \"{}\"", sanitized);
    FILE *pipe = popen(cmd.c_str(), "r");
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
		result += buffer;
	}
    std::istringstream stream(result);
    std::getline(stream, song.title);
    std::getline(stream, song.artist);
    std::getline(stream, song.thumbnail);
    std::getline(stream, song.duration);
    std::getline(stream, song.view_count);
    std::getline(stream, song.comment_count);
    std::getline(stream, song.upload_date);
    std::getline(stream, song.subscriber_count);
	std::getline(stream, song.description);
	std::getline(stream, song.channel_url);
	song.url = sanitized;
    return song;
}

std::string CringeYoutube::to_url(const std::string &query) {
    char buffer[128];
    std::string URL;
    std::string command = fmt::format(R"(yt-dlp --print webpage_url "ytsearch:{}")", query);
    const char *cmd = command.c_str();
    FILE *pipe = popen(cmd, "r");
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        URL += buffer;
    }
    return URL;
}

bool CringeYoutube::is_url(const std::string &query) {
    std::regex url_regex(R"(^(https?|ftp):\/\/[^\s/$.?#].[^\s]*)");
    return std::regex_match(query, url_regex);
}

bool CringeYoutube::is_playlist(const std::string &query) {
    std::regex playlist_regex("&(start_radio=|list=)");
    return std::regex_match(query, playlist_regex);
}

std::string CringeYoutube::search_command(std::string search) {
    return fmt::format(
        R"(yt-dlp -f bestaudio -o - -vn "{}")", search);
}

std::string CringeYoutube::search(const std::string &query) {
	std::string sanitized = this->sanitize(query);
	if(!this->is_url(sanitized)) {
		sanitized = to_url(sanitized);
	}
	if(this->is_playlist(sanitized)) {
		return "";
	}
	return this->search_command(sanitized);
}

std::string CringeYoutube::sanitize(std::string query) {
    query.erase(std::remove(query.begin(), query.end(), '\n'), query.end());
    query.erase(std::remove(query.begin(), query.end(), '"'), query.end());
    query.erase(std::remove(query.begin(), query.end(), '{'), query.end());
    query.erase(std::remove(query.begin(), query.end(), '}'), query.end());
    return query;
}

CringeYoutube::CringeYoutube() = default;

CringeYoutube::~CringeYoutube() = default;
