//
// Created by Nolan Gregory on 12/26/23.
//
#pragma once
#ifndef CRINGE_API_H
#define CRINGE_API_H

#include <dpp/dpp.h>
#include <dpp/unicode_emoji.h>

dpp::slashcommand chat_declaration();

dpp::slashcommand user_declaration();

void chat_command(dpp::cluster &bot, const dpp::slashcommand_t &event);

void user_command(dpp::cluster &bot, const dpp::slashcommand_t &event);

#endif //CRINGE_API_H
