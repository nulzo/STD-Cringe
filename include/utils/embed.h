#pragma once

#ifndef CRINGE_EMBEDS_H
#define CRINGE_EMBEDS_H

#include <dpp/dpp.h>

dpp::embed status_embed(const std::string &title, const std::string &reason, int status);
dpp::embed playing_embed(int status);
dpp::embed generic_embed(const std::string &icon, const std::string &title, const std::string &description, int status);

#endif //CRINGE_EMBEDS_H
