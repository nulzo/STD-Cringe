//
// Created by Nolan Gregory on 12/26/23.
//
#pragma once

#ifndef CRINGE_VOICE_H
#define CRINGE_VOICE_H

#include <dpp/dpp.h>
#include "utils/cringe.h"

dpp::slashcommand play_declaration();

dpp::slashcommand join_declaration();

void play_command(const dpp::cluster &bot, const dpp::slashcommand_t &event, Cringe::CringeQueue queue);

void join_command(dpp::cluster &bot, const dpp::slashcommand_t &event);

#endif //CRINGE_VOICE_H
