#ifndef TEMPLATEBOT_UTILS_H
#define TEMPLATEBOT_UTILS_H

#include <dpp/dpp.h>
#include "colors.h"
#include <iostream>
#include <ctime>
#include <curl/curl.h>
#include <string>
#include <sstream>
#include <fmt/core.h>
#include <regex>

using json = nlohmann::json;

std::string open_ai_api(const std::string &prompt, std::string max_tokens, std::string model);
int get_request(const char* request, const char* URL, const char *auth_header, std::string &response);
std::string discord_time_to_date(double timestamp);
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output);
int get_env(std::string key, std::string &value);

#endif
