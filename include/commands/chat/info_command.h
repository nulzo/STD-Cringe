//
// Created by Nolan Gregory on 3/29/24.
//

#ifndef CRINGE_INFO_COMMAND_H
#define CRINGE_INFO_COMMAND_H

#include <dpp/dpp.h>
#include <fmt/core.h>

dpp::slashcommand info_declaration();
void info_command(dpp::cluster &bot, const dpp::slashcommand_t &event);

#endif //CRINGE_INFO_COMMAND_H
