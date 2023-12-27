#ifndef CRINGE_LOGGER_H
#define CRINGE_LOGGER_H

#include <dpp/dpp.h>
#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>

const std::string MAGENTA = "\e[1;35m";
const std::string COLOR_RESET = "\e[0m";
const std::string GREEN = "\e[1;32m";
const std::string YELLOW = "\e[1;33m";
const std::string RED = "\e[1;31m";
const std::string CYAN = "\e[1;36m";
const std::string WHITE = "\e[1;37m";

std::shared_ptr<spdlog::logger> cringe_logging();

void logger(const std::shared_ptr<spdlog::logger> &log, const dpp::log_t &event);

void log_on_slash(const std::string &event, const std::string &user, const std::shared_ptr<spdlog::logger> &log);

void log_on_message_delete(const std::string &event, const std::string &user, const std::shared_ptr<spdlog::logger> &log);

void log_end_slash(const std::string &event, const std::string &user, const std::shared_ptr<spdlog::logger> &log);

void log_on_message(const std::string &event, const std::string &user, const std::shared_ptr<spdlog::logger> &log);

void log_on_start();

#endif //CRINGE_LOGGER_H
