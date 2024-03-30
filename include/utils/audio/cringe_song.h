//
// Created by Nolan Gregory on 3/29/24.
//

#ifndef CRINGE_CRINGE_SONG_H
#define CRINGE_CRINGE_SONG_H

#include <queue>
#include "iostream"
#include "dpp/dpp.h"

class CringeSong {
public:
	CringeSong(std::string title, std::string artist, std::string thumbnail, std::string duration, std::string filter, std::string url, std::string upload_date, std::string comment_count,std::string subscriber_count, std::string view_count, dpp::slashcommand_t &event);
	std::string get_title();
	std::string get_artist();
	std::string get_thumbnail();
	std::string get_formatted_duration();
	std::string get_url();
	std::string get_filter();
	std::string get_subscribers();
	std::string get_view_count();
	std::string get_comments();
	dpp::slashcommand_t get_event();
	std::string get_upload_date();
private:
	std::string title;
	std::string artist;
	std::string thumbnail;
	std::string duration;
	std::string url;
	std::string upload_date;
	std::string comment_count;
	std::string subscriber_count;
	std::string view_count;
	dpp::slashcommand_t event;
	std::string filter;
};

#endif //CRINGE_CRINGE_SONG_H
