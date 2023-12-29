//
// Created by Nolan Gregory on 12/28/23.
//

#ifndef CRINGE_PLAYCOMMAND_H
#define CRINGE_PLAYCOMMAND_H

#include <dpp/dpp.h>
#include "utils/cringe.h"

dpp::slashcommand play_declaration();
void play_callback(const std::string &song, const dpp::slashcommand_t &event);
void play_command(const dpp::cluster &bot, const dpp::slashcommand_t &event, Cringe::CringeQueue &queue);

#endif //CRINGE_PLAYCOMMAND_H
