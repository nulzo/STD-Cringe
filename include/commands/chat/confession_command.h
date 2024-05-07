#ifndef CRINGE_CONFESSION_COMMAND_H
#define CRINGE_CONFESSION_COMMAND_H

#include <dpp/dpp.h>
#include <fmt/core.h>
#include "utils/bot/cringe_bot.h"

auto confession_declaration() -> dpp::slashcommand;
void confession_command(CringeBot &cringe, const dpp::slashcommand_t &event);

#endif //CRINGE_CONFESSION_COMMAND_H
