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
#include "commands/challenge/challenge.h"
#include "commands/misc/misc.h"
#include "commands/voice/PlayCommand.h"
#include "commands/voice/JoinCommand.h"
#include "commands/voice/QueueCommand.h"
#include "commands/voice/SkipCommand.h"
#include "commands/misc/ConfessionCommand.h"
#include "commands/api/RedditCommand.h"
#include "commands/api/ImageCommand.h"
#include "commands/api/api.h"
#include "utils/logger.h"
#include "utils/cringe.h"

int main() {
	std::string BOT_TOKEN;
	Cringe::CringeQueue queue;

	log_on_start();
	get_env("BOT_TOKEN", BOT_TOKEN);

	dpp::cluster bot(BOT_TOKEN, dpp::i_default_intents | dpp::i_message_content);
	std::shared_ptr<spdlog::logger> cringe_logger = cringe_logging();

	bot.on_log([&cringe_logger](const dpp::log_t &event) {
		logger(cringe_logger, event);
	});

	bot.on_slashcommand([&bot, &cringe_logger, &queue](const dpp::slashcommand_t &event) {
		log_on_slash(event.command.get_command_name(), event.command.usr.global_name, cringe_logger);
		if (event.command.get_command_name() == "info") {
			info_command(bot, event);
		} else if (event.command.get_command_name() == "problem") {
			problem_command(bot, event);
		} else if (event.command.get_command_name() == "user") {
			user_command(bot, event);
		} else if (event.command.get_command_name() == "chat") {
			chat_command(bot, event);
		} else if (event.command.get_command_name() == "join") {
			join_command(bot, event);
		} else if (event.command.get_command_name() == "play") {
			std::thread t(play_command, std::ref(bot), event, std::ref(queue));
//			play_command(bot, event, queue);
			t.detach();
		} else if (event.command.get_command_name() == "message") {
			message_command(bot, event);
		} else if (event.command.get_command_name() == "ethan") {
			ethan_command(bot, event);
		} else if (event.command.get_command_name() == "queue") {
			queue_command(event, queue);
		} else if (event.command.get_command_name() == "skip") {
			skip_command(bot, event, queue);
		} else if (event.command.get_command_name() == "confess") {
			confession_command(bot, event);
		} else if (event.command.get_command_name() == "reddit") {
			reddit_command(bot, event);
		} else if (event.command.get_command_name() == "image") {
			image_command(bot, event);
		}
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

	bot.on_voice_track_marker([&](const dpp::voice_track_marker_t &ev) {
		if(!queue.is_empty()){
			Cringe::CringeSong s = queue.dequeue();
			play_callback(bot, s);
		}
	});

	bot.on_ready([&bot]([[maybe_unused]] const dpp::ready_t &event) {
		/* Create a new global command on ready event */
		if (dpp::run_once<struct register_bot_commands>()) {
			std::vector<dpp::slashcommand> commands{
					{
							info_declaration(),
							user_declaration(),
							problem_declaration(),
							chat_declaration(),
							join_declaration(),
							play_declaration(),
							message_declaration(),
							ethan_declaration(),
							queue_declaration(),
							skip_declaration(),
							confession_declaration(),
							reddit_declaration(),
							image_declaration()
					}
			};
			bot.global_bulk_command_create(commands);
			bot.set_presence(dpp::presence(dpp::ps_online, dpp::at_custom, "made by @nulzo"));
		}
	});

	bot.start(dpp::st_wait);

	return 0;
}