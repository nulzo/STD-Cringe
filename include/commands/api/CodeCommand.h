//
// Created by Nolan Gregory on 1/9/24.
//

#ifndef CRINGE_CODECOMMAND_H
#define CRINGE_CODECOMMAND_H

#include <dpp/dpp.h>
#include <dpp/unicode_emoji.h>

dpp::slashcommand code_declaration();
void code_command(dpp::cluster &bot, const dpp::slashcommand_t &event);

#endif //CRINGE_CODECOMMAND_H
