//
// Created by Nolan Gregory on 1/9/24.
//

#ifndef CRINGE_SLASHCOMMANDLISTENER_H
#define CRINGE_SLASHCOMMANDLISTENER_H

#include <dpp/dpp.h>
#include "utils/misc/cringe.h"

void process_slashcommand(const dpp::slashcommand_t &event,  dpp::cluster &bot, CringeQueue &queue);
void register_slashcommands(dpp::cluster &bot);

#endif //CRINGE_SLASHCOMMANDLISTENER_H
