#include <utility>
#include "utils/cringe.h"

std::string Cringe::CringeAudioSource::get_url() {
	return url;
}

dpp::slashcommand_t Cringe::CringeAudioSource::get_event() {
	return event;
}

Cringe::CringeAudioSource::CringeAudioSource(std::string url, dpp::slashcommand_t &event)
		: url(std::move(url)), event(std::move(event)) {}