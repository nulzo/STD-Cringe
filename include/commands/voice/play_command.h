//
// Created by Nolan Gregory on 12/28/23.
//

#ifndef CRINGE_PLAY_COMMAND_H
#define CRINGE_PLAY_COMMAND_H

#include <dpp/dpp.h>
#include "utils/audio/cringe_song.h"
#include "utils/audio/cringe_queue.h"


dpp::slashcommand play_declaration();
void play_callback(dpp::cluster &bot, CringeSong song);
void play_command(dpp::cluster &bot, const dpp::slashcommand_t &event, CringeQueue &queue);

#endif //CRINGE_PLAY_COMMAND_H
