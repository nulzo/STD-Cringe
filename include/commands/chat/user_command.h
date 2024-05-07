#ifndef CRINGE_USER_COMMAND_H
#define CRINGE_USER_COMMAND_H

#include <dpp/dpp.h>
#include <dpp/unicode_emoji.h>
#include "utils/bot/cringe_bot.h"

dpp::slashcommand user_declaration();
void user_command(CringeBot &cringe, const dpp::slashcommand_t &event);

#endif
