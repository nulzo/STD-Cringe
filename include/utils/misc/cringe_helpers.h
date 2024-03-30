//
// Created by Nolan Gregory on 3/29/24.
//

#ifndef CRINGE_CRINGE_HELPERS_H
#define CRINGE_CRINGE_HELPERS_H

#include <string>
#include "dpp/dpp.h"
#include "fmt/format.h"

using json = nlohmann::json;
void replace_raw_char(std::string &str, char refchar);
std::string get_env(const std::string_view &given_key);
std::string seconds_to_formatted_time(int seconds);

#endif //CRINGE_CRINGE_HELPERS_H
