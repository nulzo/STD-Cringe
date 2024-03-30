#ifndef CRINGE_CONFESSION_COMMAND_H
#define CRINGE_CONFESSION_COMMAND_H

#include <dpp/dpp.h>
#include <fmt/core.h>

dpp::slashcommand confession_declaration();
void confession_command(dpp::cluster &bot, const dpp::slashcommand_t &event);

#endif //CRINGE_CONFESSION_COMMAND_H
