#ifndef CRINGE_SKIP_COMMAND_H
#define CRINGE_SKIP_COMMAND_H

#include <dpp/dpp.h>
#include "commands/voice/skip_command.h"
#include "utils/audio/cringe_queue.h"

dpp::slashcommand skip_declaration();
void skip_command(dpp::cluster &bot, const dpp::slashcommand_t &event, CringeQueue &queue);

#endif //CRINGE_SKIP_COMMAND_H
