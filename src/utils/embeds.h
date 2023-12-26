#pragma once

#ifndef CRINGE_EMBEDS_H
#define CRINGE_EMBEDS_H

#include <dpp/dpp.h>
#include "colors.h"
#include "icon.h"

dpp::embed error_embed(const std::string &title, const std::string &description, const std::string &reason);

dpp::embed success_embed(const std::string &title, const std::string &description, const std::string &reason);

#endif //CRINGE_EMBEDS_H
