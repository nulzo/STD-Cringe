//
// Created by Nolan Gregory on 1/8/24.
//

#ifndef CRINGE_TALKCOMMAND_H
#define CRINGE_TALKCOMMAND_H

#include <dpp/dpp.h>

dpp::slashcommand talk_declaration();
void talk_command(dpp::cluster &bot, const dpp::slashcommand_t &event);

#endif //CRINGE_TALKCOMMAND_H
