//
// Created by Nolan Gregory on 3/29/24.
//

#ifndef CRINGE_CRINGE_AUDIO_HELPERS_H
#define CRINGE_CRINGE_AUDIO_HELPERS_H

#include <string>
#include <dpp/dpp.h>

std::vector<std::string> get_yt_info(std::string song);
bool is_url(const std::string &query);
std::string search_command(std::string search);
std::string sanitize_query(std::string query);
std::string query_to_url(std::string query);

#endif //CRINGE_CRINGE_AUDIO_HELPERS_H
