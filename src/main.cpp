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

dpp::cluster BOT("MTE4Njg2MDMzMjg0NTYyOTUxMQ.G2u0_7.143KQo9IX4-KBKLYCEUYqbDfm8f4d5yiozMvWs", dpp::i_default_intents | dpp::i_message_content);

// Function to handle the SIGINT signal
void handle_signal(int signal) {
	BOT.shutdown();
	BOT.log(dpp::loglevel::ll_critical, "Caught signal to shutdown. Exiting gracefully.");
	exit(signal);
}

int main() {
	// Register the signal handler for SIGTERM
	signal(SIGTERM, handle_signal);

	Cringe::CringeQueue queue;
	log_on_start();
	std::shared_ptr<spdlog::logger> cringe_logger = cringe_logging();

	BOT.on_log([&cringe_logger](const dpp::log_t &event) {
		logger(cringe_logger, event);
	});

	BOT.on_slashcommand([&cringe_logger, &queue](const dpp::slashcommand_t &event) {
		log_on_slash(event.command.get_command_name(), event.command.usr.global_name, cringe_logger);
		process_slashcommand(event, BOT, queue);
		log_end_slash(event.command.get_command_name(), event.command.usr.global_name, cringe_logger);
	});

	BOT.on_message_delete([&cringe_logger](const dpp::message_delete_t &event) {
		log_on_message_delete(event.raw_event, event.raw_event, cringe_logger);
	});

	BOT.on_message_create([&cringe_logger](const dpp::message_create_t &event) {
		log_on_message(event.msg.content, event.msg.author.global_name, cringe_logger);
		/* get message to edit it after */
		const dpp::snowflake msg_id = event.msg.id;
		/* here string will automatically be converted to snowflake */
		BOT.message_get(msg_id, event.msg.channel_id, [](const dpp::confirmation_callback_t &callback) {
			auto message = callback.get<dpp::message>();
			if (message.author.id == 405912283554906124 || message.author.id == 411399698679595009) {
				/* change the message content and edit the message itself */
				BOT.message_delete(message.id, message.channel_id);
				BOT.direct_message_create(message.author.id, (const dpp::message) "Nice try");
			}
		});
	});

	BOT.on_voice_track_marker([&queue](const dpp::voice_track_marker_t &ev) {
		if (!queue.is_empty()) {
			Cringe::CringeSong s = queue.dequeue();
			play_callback(BOT, s);
		}
	});

	BOT.on_ready([]([[maybe_unused]] const dpp::ready_t &event) {
		/* Create a new global command on ready event */
		if (dpp::run_once<struct register_BOT_commands>()) {
			register_slashcommands(BOT);
		}
	});

	BOT.start(dpp::st_wait);
	return 0;
}