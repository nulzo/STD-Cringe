//
// Created by Nolan Gregory on 12/28/23.
//

#ifndef CRINGE_QUEUE_COMMAND_H
#define CRINGE_QUEUE_COMMAND_H

#include <dpp/dpp.h>
#include "utils/misc/cringe.h"
#include "utils/audio/cringe_queue.h"

dpp::slashcommand queue_declaration();
void queue_command(const dpp::slashcommand_t &event, CringeQueue queue);

#endif //CRINGE_QUEUE_COMMAND_H
