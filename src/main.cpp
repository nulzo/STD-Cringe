/*
 * MIT License
 *
 * Copyright (c) 2023 @nulzo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "utils/util.h"
#include "commands/voice/PlayCommand.h"
#include "utils/logger.h"
#include "utils/cringe.h"
#include <csignal>
#include "listeners/SlashcommandListener.h"

// Function to handle the SIGINT signal
void handle_signal(int signal) {
	std::cout << "Caught SIGINT signal (" << signal << "). Exiting gracefully." << std::endl;
	// Additional cleanup or handling code can be added here
	exit(signal);
}

int main() {
	std::string BOT_TOKEN;
	get_env("BOT_TOKEN", BOT_TOKEN);
	dpp::cluster bot(BOT_TOKEN, dpp::i_default_intents | dpp::i_message_content);
	Cringe::CringeQueue queue;
	log_on_start();
	// Register the signal handler for SIGTERM
	signal(SIGTERM, handle_signal);
	std::shared_ptr<spdlog::logger> cringe_logger = cringe_logging();

	bot.on_log([&cringe_logger](const dpp::log_t &event) {
		logger(cringe_logger, event);
	});

	bot.on_slashcommand([&bot, &cringe_logger, &queue](const dpp::slashcommand_t &event) {
		log_on_slash(event.command.get_command_name(), event.command.usr.global_name, cringe_logger);
		process_slashcommand(event, bot, queue);
		log_end_slash(event.command.get_command_name(), event.command.usr.global_name, cringe_logger);
	});

	bot.on_message_delete([&cringe_logger](const dpp::message_delete_t &event) {
		log_on_message_delete(event.raw_event, event.raw_event, cringe_logger);
	});

	bot.on_message_create([&bot, &cringe_logger](const dpp::message_create_t &event) {
		log_on_message(event.msg.content, event.msg.author.global_name, cringe_logger);
		/* get message to edit it after */
		const dpp::snowflake msg_id = event.msg.id;
		/* here string will automatically be converted to snowflake */
		bot.message_get(msg_id, event.msg.channel_id, [&bot](const dpp::confirmation_callback_t &callback) {
			auto message = callback.get<dpp::message>();
			if (message.author.id == 405912283554906124 || message.author.id == 411399698679595009) {
				/* change the message content and edit the message itself */
				bot.message_delete(message.id, message.channel_id);
				bot.direct_message_create(message.author.id, (const dpp::message) "Nice try");
			}
		});
	});

	bot.on_voice_track_marker([&queue, &bot](const dpp::voice_track_marker_t &ev) {
		if (!queue.is_empty()) {
			Cringe::CringeSong s = queue.dequeue();
			play_callback(bot, s);
		}
	});

	bot.on_ready([&bot]([[maybe_unused]] const dpp::ready_t &event) {
		/* Create a new global command on ready event */
		if (dpp::run_once<struct register_bot_commands>()) {
			register_slashcommands(bot);
		}
	});

	bot.start(dpp::st_wait);
	return 0;
}