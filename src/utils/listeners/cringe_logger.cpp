/*
 * MIT License
 *
 * Copyright (c) 2023 @nulzo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "utils/listeners/cringe_logger.h"

CringeLogger::CringeLogger() {
	const int t_pool_sz = 8192;
	const int t_pool_snk = 5242880;
	const int t_pool_cnt = 10;
	const std::string log_name = "std_cringe.log";
	std::vector<spdlog::sink_ptr> sinks;
	spdlog::init_thread_pool(t_pool_sz, 2);
	auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	auto rotating = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(log_name, t_pool_snk, t_pool_cnt);
	sinks.push_back(stdout_sink);
	sinks.push_back(rotating);
	this->log = std::make_shared<spdlog::async_logger>("| cringe |", sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
	spdlog::register_logger(this->log);
	this->log->set_pattern("%n :: %H:%M:%S :: %^[%=9l]%$ :: %v");
	this->log->set_level(spdlog::level::level_enum::debug);
}

CringeLogger::~CringeLogger() = default;

auto CringeLogger::color_message(const std::string &color, const std::string &message) -> std::string {
	return fmt::format("{}{}{}", color, message, this->COLOR_RESET);
}

void CringeLogger::log_event(const dpp::log_t &event) {
	switch (event.severity) {
		case dpp::ll_trace:
			this->log->trace("{}", this->color_message(this->WHITE, event.message));
			break;
		case dpp::ll_debug:
			this->log->debug("{}", this->color_message(this->WHITE, event.message));
			break;
		case dpp::ll_info:
			this->log->info("{}", this->color_message(this->WHITE, event.message));
			break;
		case dpp::ll_warning:
			this->log->warn("{}", event.message);
			break;
		case dpp::ll_error:
			this->log->error("{}{}{}", this->RED, event.message, this->COLOR_RESET);
			break;
		case dpp::ll_critical:
			this->log->critical("{}{}{}", this->RED, event.message, this->COLOR_RESET);
			break;
		default:
			this->log->info("{}", event.message);
			break;
	}
}

void CringeLogger::log_start() {
	const int box_width = 100;
    fmt::print("┌{0:─^{2}}┐\n│{1: ^{2}}│\n│{3: ^{2}}│\n└{0:─^{2}}┘\n", "","std::cringe", box_width, "version 0.0.1");
}

void CringeLogger::log_slashcommand(const dpp::slashcommand_t &event) {
	this->log->debug("{} {} {}", color_message(this->YELLOW, event.command.usr.username),
					 color_message(this->CYAN, "used slashcommand"),
					 color_message(this->MAGENTA, event.command.get_command_name()));
}

void CringeLogger::log_message_delete(const dpp::message_delete_t &event) {
	this->log->debug(fmt::format("message deleted in channel {}", color_message(this->RED, event.from->creator->channel_get_sync(event.channel_id).name)));
}

void CringeLogger::log_message(const dpp::message_create_t &event) {
	if (!event.msg.author.is_bot()) {
		this->log->debug("{} {} {}", color_message(this->YELLOW, event.msg.author.username),
						 color_message(this->CYAN, "sent message"),
						 color_message(this->MAGENTA, event.msg.content));
	}
}
