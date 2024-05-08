#ifndef CRINGE_CHAT_COMMAND_H
#define CRINGE_CHAT_COMMAND_H

#include <dpp/dpp.h>
#include <dpp/unicode_emoji.h>
#include "utils/bot/cringe_bot.h"

dpp::slashcommand chat_declaration(CringeBot &cringe);
void chat_command(CringeBot &cringe, const dpp::slashcommand_t &event);

#endif
