#ifndef CRINGE_CRINGE_LOGGER_H
#define CRINGE_CRINGE_LOGGER_H

#include <dpp/dpp.h>
#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>


class CringeLogger {
public:
	const std::string MAGENTA = "\e[1;35m";
	const std::string COLOR_RESET = "\e[0m";
	const std::string GREEN = "\e[1;32m";
	const std::string YELLOW = "\e[1;33m";
	const std::string RED = "\e[1;31m";
	const std::string CYAN = "\e[1;36m";
	const std::string WHITE = "\e[1;37m";
	CringeLogger();
	~CringeLogger();
	void log_event(const dpp::log_t &event);
	void log_slashcommand(const dpp::slashcommand_t &event);
	void log_message_delete(const dpp::message_delete_t &event);
	void log_message(const dpp::message_create_t &event);
	void log_start();
private:
	std::shared_ptr<spdlog::logger> log;
	auto color_message(const std::string &color, const std::string &message) -> std::string;
};

#endif
