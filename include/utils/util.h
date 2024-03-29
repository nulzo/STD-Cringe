#pragma once

#ifndef CRINGE_UTILS_H
#define CRINGE_UTILS_H

#include <dpp/dpp.h>
#include <iostream>
#include <curl/curl.h>
#include <string>
#include <fmt/core.h>
#include <regex>
#include "cringe.h"

using json = nlohmann::json;

std::string get_image(const std::string &prompt);

json post(const std::string &request, const std::string &endpoint);

std::string get_openai_response(const std::string &prompt, std::string max_tokens, std::string model);

std::string get_tts_response(const std::string &prompt);

std::string get_ollama_chat(const std::string &prompt);

json get_chat(const std::string &prompt, const std::string &model);

std::string get_ollama_code(const std::string &prompt);

std::string get_ollama_ethan(const std::string &prompt);

std::string get_ollama_klim(const std::string &prompt);

std::string discord_time_to_date(double timestamp);

size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *output);

inline std::string get_env(const std::string_view &given_key);

std::string seconds_to_formatted_time(int seconds);

void voice_threading(std::function<void(dpp::cluster&, const dpp::slashcommand_t&, Cringe::CringeQueue&)> process, dpp::cluster& bot, const dpp::slashcommand_t& event, Cringe::CringeQueue& queue);

std::string get_reddit_response(const std::string &subreddit, const std::string &filter);

#endif
