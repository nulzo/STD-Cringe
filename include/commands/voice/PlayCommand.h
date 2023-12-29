//
// Created by Nolan Gregory on 12/28/23.
//

#ifndef CRINGE_PLAYCOMMAND_H
#define CRINGE_PLAYCOMMAND_H

#include <dpp/dpp.h>
#include "utils/cringe.h"

dpp::slashcommand play_declaration();
void play_callback(dpp::cluster &bot, Cringe::CringeSong song);
void play_command(dpp::cluster &bot, const dpp::slashcommand_t &event, Cringe::CringeQueue &queue);

#endif //CRINGE_PLAYCOMMAND_H
