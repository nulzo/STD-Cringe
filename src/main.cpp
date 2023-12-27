#include "utils/util.h"
#include "commands/challenge.h"
#include "commands/misc.h"
#include "commands/voice.h"
#include "commands/api.h"
#include "utils/logger.h"

int main() {
	std::string BOT_TOKEN;
	log_on_start();
	get_env("BOT_TOKEN", BOT_TOKEN);

	dpp::cluster bot(BOT_TOKEN, dpp::i_default_intents | dpp::i_message_content);
	std::shared_ptr<spdlog::logger> cringe_logger = cringe_logging();

	bot.on_log([&cringe_logger](const dpp::log_t &event) {
		logger(cringe_logger, event);
	});

	bot.on_slashcommand([&bot, &cringe_logger](const dpp::slashcommand_t &event) {
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
			play_command(bot, event);
		} else if (event.command.get_command_name() == "message") {
			message_command(bot, event);
		} else if (event.command.get_command_name() == "ethan") {
			ethan_command(bot, event);
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
							ethan_declaration()
					}
			};
			bot.global_bulk_command_create(commands);
			bot.set_presence(dpp::presence(dpp::ps_online, dpp::at_custom, "made by @nulzo"));
		}
	});

	bot.start(dpp::st_wait);
	return 0;
}