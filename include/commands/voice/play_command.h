#ifndef CRINGE_PLAY_COMMAND_H
#define CRINGE_PLAY_COMMAND_H

#include <dpp/dpp.h>
#include "utils/audio/cringe_song.h"
#include "utils/audio/cringe_queue.h"
#include "utils/bot/cringe_bot.h"


dpp::slashcommand play_declaration();
void play_callback(CringeBot &cringe, CringeSong song);
void play_command(CringeBot &cringe, const dpp::slashcommand_t &event);

#endif
