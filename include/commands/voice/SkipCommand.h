//
// Created by Nolan Gregory on 12/29/23.
//

#ifndef CRINGE_SKIPCOMMAND_H
#define CRINGE_SKIPCOMMAND_H

#include <dpp/dpp.h>
#include "utils/cringe.h"

dpp::slashcommand skip_declaration();
void skip_command(dpp::cluster &bot, const dpp::slashcommand_t &event, Cringe::CringeQueue &queue);

#endif //CRINGE_SKIPCOMMAND_H
