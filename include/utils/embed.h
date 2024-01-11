#pragma once

#ifndef CRINGE_EMBEDS_H
#define CRINGE_EMBEDS_H

#include <dpp/dpp.h>
#include "cringe.h"

dpp::embed status_embed(const std::string &title, const std::string &reason, int status);
dpp::embed playing_embed(int status);
dpp::embed now_streaming(Cringe::CringeSong song);
dpp::embed info_embed(const std::string& title, const std::string& response, const std::string& avatar_url, const std::string& mention, const std::string& created, const std::string& joined_at, const std::string& premium, const std::string& nitro, const std::string& bot);
dpp::embed generic_embed(const std::string &icon, const std::string &title, const std::string &description, int status);
dpp::embed added_to_queue_embed(Cringe::CringeSong song);
dpp::embed confession_embed(const std::string& confession);
dpp::embed reddit_embed(nlohmann::json data);

#endif //CRINGE_EMBEDS_H
