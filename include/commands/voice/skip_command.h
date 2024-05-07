#ifndef CRINGE_SKIP_COMMAND_H
#define CRINGE_SKIP_COMMAND_H

#include <dpp/dpp.h>
#include "commands/voice/skip_command.h"
#include "utils/audio/cringe_queue.h"
#include "utils/bot/cringe_bot.h"

dpp::slashcommand skip_declaration();
void skip_command(CringeBot &cringe, const dpp::slashcommand_t &event);

#endif //CRINGE_SKIP_COMMAND_H
