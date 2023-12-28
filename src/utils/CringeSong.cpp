#include <utility>

#include "utils/cringe.h"

std::string Cringe::CringeSong::get_title() {
	return title;
}

std::string Cringe::CringeSong::get_artist() {
	return artist;
}

std::string Cringe::CringeSong::get_thumbnail() {
	return thumbnail;
}

std::string Cringe::CringeSong::get_formatted_duration() {
	return duration;
}

void Cringe::CringeSong::set_title(std::string t) {
	title = std::move(t);
}

void Cringe::CringeSong::set_artist(std::string a) {
	artist = std::move(a);
}

void Cringe::CringeSong::set_thumbnail(std::string tn) {
	thumbnail = std::move(tn);
}

void Cringe::CringeSong::set_formatted_duration(std::string fd) {
	duration = std::move(fd);
}

Cringe::CringeSong::CringeSong(std::string title, std::string artist, std::string thumbnail, std::string duration)
		: title(std::move(title)), artist(std::move(artist)), thumbnail(std::move(thumbnail)), duration(std::move(duration)) {}

