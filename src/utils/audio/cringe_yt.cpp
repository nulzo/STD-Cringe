#include "utils/audio/cringe_yt.h"

Song CringeYoutube::get_content(const std::string &query) {
	Song song;
	char buffer[128];
	std::string result;
	std::string cmd = fmt::format("yt-dlp -s --print title --print channel --print thumbnail --print duration --print view_count --print comment_count --print epoch --print channel_follower_count \"{}\"", query);
	FILE *pipe = popen(cmd.c_str(), "r");
	while (fgets(buffer, sizeof(buffer), pipe) != nullptr) result += buffer;
	std::istringstream stream(result);
	std::getline(stream, song.title);
	std::getline(stream, song.artist);
	std::getline(stream, song.thumbnail);
	std::getline(stream, song.duration);
	std::getline(stream, song.view_count);
	std::getline(stream, song.comment_count);
	std::getline(stream, song.upload_date);
	std::getline(stream, song.subscriber_count);
	return song;
}

std::string CringeYoutube::to_url(const std::string &query) {
	char buffer[128];
	std::string URL;
	std::string command = fmt::format("yt-dlp --print webpage_url \"ytsearch:{}\"", query);
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
	return fmt::format(R"(yt-dlp -f bestaudio -o - -vn "{}" | ffmpeg -i pipe:0 -hide_banner -loglevel warning -f s16le -ac 2 -ar 48000 pipe:1)", search);
}

std::string CringeYoutube::sanitize(std::string query) {
	query.erase(std::remove(query.begin(), query.end(), '\n'), query.end());
	query.erase(std::remove(query.begin(), query.end(), '"'), query.end());
	query.erase(std::remove(query.begin(), query.end(), '{'), query.end());
	query.erase(std::remove(query.begin(), query.end(), '}'), query.end());
	return query;
}
