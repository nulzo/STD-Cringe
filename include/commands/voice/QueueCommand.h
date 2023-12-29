//
// Created by Nolan Gregory on 12/28/23.
//

#ifndef CRINGE_QUEUECOMMAND_H
#define CRINGE_QUEUECOMMAND_H

#include <dpp/dpp.h>
#include "utils/cringe.h"

dpp::slashcommand queue_declaration();
void queue_command(const dpp::slashcommand_t &event, Cringe::CringeQueue queue);

#endif //CRINGE_QUEUECOMMAND_H
