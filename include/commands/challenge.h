//
// Created by Nolan Gregory on 12/26/23.
//

#ifndef CRINGE_CHALLENGE_H
#define CRINGE_CHALLENGE_H

#include <dpp/dpp.h>
#include <random>
#include <regex>

dpp::slashcommand problem_declaration();

void problem_command(dpp::cluster &bot, const dpp::slashcommand_t &event);

#endif //CRINGE_CHALLENGE_H
