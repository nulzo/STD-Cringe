#ifndef CRINGE_CRINGE_SONG_NEW_H
#define CRINGE_CRINGE_SONG_NEW_H

#include "iostream"
#include "dpp/dpp.h"
#include "utils/audio/cringe_song_new.h"

struct Song {
	std::string subscriber_count;
	std::string comment_count;
	std::string upload_date;
	std::string view_count;
	std::string thumbnail;
	std::string duration;
	std::string artist;
	std::string filter;
	std::string title;
	std::string url;
};

#endif
