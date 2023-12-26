#pragma once

#ifndef CRINGE_COMMANDS_H
#define CRINGE_COMMANDS_H

#include <dpp/dpp.h>
#include <curl/curl.h>
#include <iostream>
#include "../utils/utils.h"
#include "dpp/nlohmann/json.hpp"
#include <fstream>
#include <random>
#include <string>
#include <sstream>
#include <regex>
#include <dpp/unicode_emoji.h>
#include <fmt/core.h>
#include <mpg123.h>
#include <out123.h>
#include <iomanip>
#include <vector>

// So we can avoid retyping things often ;o)
using json = nlohmann::json;

// Functions that define the slash commands presentation
dpp::slashcommand chat_declaration();

dpp::slashcommand info_declaration();

dpp::slashcommand user_declaration();

dpp::slashcommand problem_declaration();

dpp::slashcommand message_declaration();

dpp::slashcommand ethan_declaration();

// Functions that define the logic of the slash commands
void user_command(dpp::cluster &bot, const dpp::slashcommand_t &event);

void info_command(dpp::cluster &bot, const dpp::slashcommand_t &event);

void play_command(const dpp::cluster &bot, const dpp::slashcommand_t &event);

void join_command(dpp::cluster &bot, const dpp::slashcommand_t &event);

void problem_command(dpp::cluster &bot, const dpp::slashcommand_t &event);

void chat_command(dpp::cluster &bot, const dpp::slashcommand_t &event);

void message_command(dpp::cluster &bot, const dpp::slashcommand_t &event);

void ethan_command(dpp::cluster &bot, const dpp::slashcommand_t &event);

#endif
