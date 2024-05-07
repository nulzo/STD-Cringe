#ifndef CRINGE_INFO_COMMAND_H
#define CRINGE_INFO_COMMAND_H

#include <dpp/dpp.h>
#include <fmt/core.h>
#include "utils/bot/cringe_bot.h"

dpp::slashcommand info_declaration();
void info_command(CringeBot &cringe, const dpp::slashcommand_t &event);

#endif
