#include "utils/logger.h"

std::shared_ptr<spdlog::logger> cringe_logging() {
	const std::string log_name = "std_cringe.log";
	std::vector<spdlog::sink_ptr> sinks;
	static std::shared_ptr<spdlog::logger> log;
	spdlog::init_thread_pool(8192, 2);
	auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	auto rotating = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(log_name, 1024 * 1024 * 5, 10);
	sinks.push_back(stdout_sink);
	sinks.push_back(rotating);
	log = std::make_shared<spdlog::async_logger>("| cringe |", sinks.begin(), sinks.end(), spdlog::thread_pool(),
												 spdlog::async_overflow_policy::block);
	spdlog::register_logger(log);
	log->set_pattern("%n :: %H:%M:%S :: %^[%=9l]%$ :: %v");
	log->set_level(spdlog::level::level_enum::debug);
	return log;
}

std::string color_general(const std::string &event) {
	return fmt::format("{}{}{}", WHITE, event, COLOR_RESET);
}

void logger(const std::shared_ptr<spdlog::logger> &log, const dpp::log_t &event) {
	switch (event.severity) {
		case dpp::ll_trace:
			log->trace("{}", color_general(event.message));
			break;
		case dpp::ll_debug:
			log->debug("{}", color_general(event.message));
			break;
		case dpp::ll_info:
			log->info("{}", color_general(event.message));
			break;
		case dpp::ll_warning:
			log->warn("{}", event.message);
			break;
		case dpp::ll_error:
			log->error("{}{}{}", RED, event.message, COLOR_RESET);
			break;
		case dpp::ll_critical:
			log->critical("{}{}{}", RED, event.message, COLOR_RESET);
			break;
		default:
			log->info("{}", event.message);
			break;
	}
}

std::string color_user(const std::string &user) {
	return fmt::format("{}{}{}", YELLOW, user, COLOR_RESET);
}

std::string color_slash(const std::string &slash) {
	return fmt::format("{}\\{}{}", MAGENTA, slash, COLOR_RESET);
}

std::string color_message(const std::string &message) {
	return fmt::format("{}{}{}", YELLOW, message, COLOR_RESET);
}

std::string color_debug(const std::string &message) {
	return fmt::format("{}{}{}", CYAN, message, COLOR_RESET);
}

std::string color_info(const std::string &message) {
	return fmt::format("{}{}{}", GREEN, message, COLOR_RESET);
}

void log_on_start() {
	fmt::print("┌{0:─^{2}}┐\n│{1: ^{2}}│\n│{3: ^{2}}│\n└{0:─^{2}}┘\n", "", "std::cringe", 100, "version 0.0.1");
}

void log_on_slash(const std::string &event, const std::string &user, const std::shared_ptr<spdlog::logger> &log) {
	log->debug("{} {} {}", color_user(user), color_debug("called command:"), color_slash(event));
}

void log_end_slash(const std::string &event, const std::string &user, const std::shared_ptr<spdlog::logger> &log) {
	log->debug("{} {} {}", color_user(user), color_debug("command ended:"), color_slash(event));
}

void log_on_message(const std::string &event, const std::string &user, const std::shared_ptr<spdlog::logger> &log) {
	log->info("{} {} {}", color_user(user), color_info("sent a message:"), color_message(event));
}

void log_on_message_delete(const std::string &event, const std::string &user, const std::shared_ptr<spdlog::logger> &log) {
	log->error("{}{} deleted a message: {}{}", RED, user, event, COLOR_RESET);
}