#ifndef CRINGE_CRINGE_SLASHCOMMAND_H
#define CRINGE_CRINGE_SLASHCOMMAND_H

#include "utils/bot/cringe_bot.h"
#include <dpp/dpp.h>

void process_slashcommand(const dpp::slashcommand_t &event, CringeBot &cringe);
void register_slashcommands(CringeBot &cringe);

#endif
