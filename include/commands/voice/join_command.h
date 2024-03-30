//
// Created by Nolan Gregory on 12/28/23.
//

#ifndef CRINGE_JOIN_COMMAND_H
#define CRINGE_JOIN_COMMAND_H

#include <dpp/dpp.h>

dpp::slashcommand join_declaration();
void join_command(dpp::cluster &bot, const dpp::slashcommand_t &event);

#endif //CRINGE_JOIN_COMMAND_H
