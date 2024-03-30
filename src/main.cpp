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

#include "commands/voice/play_command.h"
#include "utils/cringe_logger.h"
#include "utils/misc/cringe.h"
#include "utils/misc/cringe_helpers.h"
#include "listeners/SlashcommandListener.h"

int main() {
	dpp::cluster BOT(get_env("BOT_TOKEN"), dpp::i_default_intents | dpp::i_message_content);

	CringeQueue queue;
	log_on_start();
	std::shared_ptr<spdlog::logger> cringe_logger = cringe_logging();

	BOT.on_log([&cringe_logger](const dpp::log_t &event) {
		logger(cringe_logger, event);
	});

	BOT.on_slashcommand([&BOT, &cringe_logger, &queue](const dpp::slashcommand_t &event) {
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
		// if(event.msg.author.id == 405912283554906123) {
		// 	std::string response = get_ollama_chat(fmt::format("In one sentence, respond to Klim who said - '{}'", event.msg.content));
		//	dpp::message msg(event.msg.channel_id, response);
		//	event.reply(msg);
		//}
	});

	BOT.on_voice_track_marker([&BOT, &queue](const dpp::voice_track_marker_t &ev) {
		if (!queue.is_empty()) {
			CringeSong s = queue.dequeue();
			play_callback(BOT, s);
		}
	});

	BOT.on_ready([&BOT]([[maybe_unused]] const dpp::ready_t &event) {
		/* Create a new global command on ready event */
		if (dpp::run_once<struct register_BOT_commands>()) {
			register_slashcommands(BOT);
		}
	});

	BOT.start(dpp::st_wait);
	return 0;
}
