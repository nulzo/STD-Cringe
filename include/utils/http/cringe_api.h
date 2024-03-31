#ifndef CRINGE_CRINGE_API_H
#define CRINGE_CRINGE_API_H

#include <dpp/dpp.h>
#include <iostream>
#include <string>
#include <fmt/core.h>
#include <regex>

using json = nlohmann::json;

json cringe_chat(const std::string &prompt, const std::string &model);
json cringe_history(const std::string &prompt, const std::string &model);
json cringe_describe(const std::string &url);
json cringe_imagine(const std::string &prompt, const std::string &style);
std::string get_reddit_response(const std::string &subreddit, const std::string &filter);

#endif //CRINGE_CRINGE_API_H
