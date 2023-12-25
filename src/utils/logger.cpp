#include "logger.h"

std::shared_ptr<spdlog::logger> cringe_logging() {
    const std::string log_name = "std_cringe.log";
    std::vector<spdlog::sink_ptr> sinks;
    static std::shared_ptr<spdlog::logger> log;
    spdlog::init_thread_pool(8192, 2);
    auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt >();
    auto rotating = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(log_name, 1024 * 1024 * 5, 10);
    sinks.push_back(stdout_sink);
    sinks.push_back(rotating);
    log = std::make_shared<spdlog::async_logger>("| cringe |", sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
    spdlog::register_logger(log);
    log->set_pattern("%n :: %H:%M:%S :: %^[%=9l]%$ :: %v");
    log->set_level(spdlog::level::level_enum::debug);
    return log;
}

void logger(const std::shared_ptr<spdlog::logger> &log, const dpp::log_t & event) {
    switch (event.severity) {
        case dpp::ll_trace:
            log->trace("{}", event.message);
            break;
        case dpp::ll_debug:
            log->debug("{}", event.message);
            break;
        case dpp::ll_info:
            log->info("{}", event.message);
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

void log_on_start(){
    fmt::print("┌{0:─^{2}}┐\n│{1: ^{2}}│\n│{3: ^{2}}│\n└{0:─^{2}}┘\n", "", "std::cringe", 80, "version 0.0.1");
}

void log_on_slash(std::string event, std::string user, const std::shared_ptr<spdlog::logger> &log) {
    log->warn("{}{} called {} command{}", YELLOW, user, event, COLOR_RESET);
}

void log_end_slash(std::string event, std::string user, const std::shared_ptr<spdlog::logger> &log) {
    log->info("{}{} command called by {} has finished{}", GREEN, event, user, COLOR_RESET);
}

void log_on_message(const std::string& event, const std::string& user, const std::shared_ptr<spdlog::logger> &log) {
    log->info("{}{} sent a message: {}{}{}{}", GREEN, user, COLOR_RESET, MAGENTA, event, COLOR_RESET);
}

void log_on_message_delete(std::string event, std::string user, const std::shared_ptr<spdlog::logger> &log) {
    log->error("{}{} deleted a message: {}{}", RED, user, event, COLOR_RESET);
}