//
// Created by Nolan Gregory on 12/26/23.
//

#ifndef CRINGE_MISC_H
#define CRINGE_MISC_H

#include <dpp/dpp.h>
#include <fmt/core.h>

dpp::slashcommand info_declaration();

dpp::slashcommand message_declaration();

dpp::slashcommand ethan_declaration();

void info_command(dpp::cluster &bot, const dpp::slashcommand_t &event);

void message_command(dpp::cluster &bot, const dpp::slashcommand_t &event);

void ethan_command(dpp::cluster &bot, const dpp::slashcommand_t &event);

#endif //CRINGE_MISC_H
