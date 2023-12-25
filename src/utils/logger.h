#ifndef CRINGE_LOGGER_H
#define CRINGE_LOGGER_H

#define BMAG "\e[1;35m"
#define COLOR_RESET "\e[0m"
#define BGRN "\e[1;32m"
#define BYEL "\e[1;33m"
#define BRED "\e[1;31m"

#include <dpp/dpp.h>
#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>

std::shared_ptr<spdlog::logger> cringe_logging();
void logger(std::shared_ptr<spdlog::logger> &log, const dpp::log_t &event);
void log_on_slash(std::string event, std::string user, std::shared_ptr<spdlog::logger> &log);
void log_end_slash(std::string event, std::string user, std::shared_ptr<spdlog::logger> &log);
void log_on_message(const std::string& event, const std::string& user, std::shared_ptr<spdlog::logger> &log);
void log_on_start();

#endif //CRINGE_LOGGER_H
