//
// Created by Nolan Gregory on 1/9/24.
//

#ifndef CRINGE_KLIMCOMMAND_H
#define CRINGE_KLIMCOMMAND_H

#include <dpp/dpp.h>
#include <dpp/unicode_emoji.h>

dpp::slashcommand klim_declaration();
void klim_command(dpp::cluster &bot, const dpp::slashcommand_t &event);

#endif //CRINGE_KLIMCOMMAND_H
