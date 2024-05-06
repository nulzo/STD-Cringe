#ifndef CRINGE_SLASHCOMMANDLISTENER_H
#define CRINGE_SLASHCOMMANDLISTENER_H

#include "utils/bot/cringe_bot.h"
#include <dpp/dpp.h>

void process_slashcommand(const dpp::slashcommand_t &event, CringeBot &cringe);
void register_slashcommands(dpp::cluster &bot);

#endif
