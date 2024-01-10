//
// Created by Nolan Gregory on 1/9/24.
//

#ifndef CRINGE_USERCOMMAND_H
#define CRINGE_USERCOMMAND_H

#include <dpp/dpp.h>
#include <dpp/unicode_emoji.h>

dpp::slashcommand user_declaration();
void user_command(dpp::cluster &bot, const dpp::slashcommand_t &event);

#endif //CRINGE_USERCOMMAND_H
