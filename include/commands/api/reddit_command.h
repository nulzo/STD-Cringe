#ifndef CRINGE_REDDIT_COMMAND_H
#define CRINGE_REDDIT_COMMAND_H

#include <dpp/dpp.h>
#include "utils/bot/cringe_bot.h"

dpp::slashcommand reddit_declaration();
void reddit_command(CringeBot &cringe, const dpp::slashcommand_t &event);

#endif //CRINGE_REDDIT_COMMAND_H
