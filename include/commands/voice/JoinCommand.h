//
// Created by Nolan Gregory on 12/28/23.
//

#ifndef CRINGE_JOINCOMMAND_H
#define CRINGE_JOINCOMMAND_H

#include <dpp/dpp.h>

dpp::slashcommand join_declaration();
void join_command(dpp::cluster &bot, const dpp::slashcommand_t &event);

#endif //CRINGE_JOINCOMMAND_H
