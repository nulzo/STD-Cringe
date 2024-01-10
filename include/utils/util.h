#pragma once

#ifndef CRINGE_UTILS_H
#define CRINGE_UTILS_H

#include <dpp/dpp.h>
#include <iostream>
#include <curl/curl.h>
#include <string>
#include <fmt/core.h>
#include <regex>

using json = nlohmann::json;

std::string get_openai_response(const std::string &prompt, std::string max_tokens, std::string model);

std::string get_tts_response(const std::string &prompt);

std::string get_ollama_response(const std::string &prompt);

std::string discord_time_to_date(double timestamp);

size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *output);

int get_env(const std::string_view &given_key, std::string &value);

std::string seconds_to_formatted_time(int seconds);

#endif
