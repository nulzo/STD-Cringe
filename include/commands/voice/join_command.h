#ifndef CRINGE_JOIN_COMMAND_H
#define CRINGE_JOIN_COMMAND_H

#include <dpp/dpp.h>
#include "utils/bot/cringe_bot.h"

dpp::slashcommand join_declaration();
void join_command(CringeBot &cringe, const dpp::slashcommand_t &event);

#endif
