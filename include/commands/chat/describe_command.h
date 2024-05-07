#ifndef CRINGE_DESCRIBE_COMMAND_H
#define CRINGE_DESCRIBE_COMMAND_H

#include <dpp/dpp.h>
#include <dpp/unicode_emoji.h>
#include "utils/bot/cringe_bot.h"

dpp::slashcommand describe_declaration();
void describe_command(CringeBot &cringe, const dpp::slashcommand_t &event);

#endif
