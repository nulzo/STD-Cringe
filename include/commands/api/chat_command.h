//
// Created by Nolan Gregory on 1/9/24.
//

#ifndef CRINGE_CHAT_COMMAND_H
#define CRINGE_CHAT_COMMAND_H

#include <dpp/dpp.h>
#include <dpp/unicode_emoji.h>

dpp::slashcommand chat_declaration();
void chat_command(dpp::cluster &bot, const dpp::slashcommand_t &event);

#endif //CRINGE_CHAT_COMMAND_H
