#include "commands/api.h"
#include "utils/cringe.h"
#include "utils/util.h"

dpp::slashcommand chat_declaration() {
	return dpp::slashcommand()
			.set_name("chat")
			.set_description("Chat with cringe")
			.add_option(dpp::command_option(dpp::co_string, "prompt", "What to ask cringe", true));
}

dpp::slashcommand user_declaration() {
	return dpp::slashcommand()
			.set_name("user")
			.set_description("Get general information about a user")
			.add_option(dpp::command_option(dpp::co_user, "username", "The user to view", true));
}

void chat_command(dpp::cluster &bot, const dpp::slashcommand_t &event) {
	std::string prompt = std::get<std::string>(event.get_parameter("prompt"));
	event.thinking();
	std::string response = open_ai_api(prompt, "250", "gpt-3.5-turbo-1106");
	event.edit_original_response(response);
}

void user_command(dpp::cluster &bot, const dpp::slashcommand_t &event) {
	std::string prompt;
	std::string user_username;
	std::string user_avatar;
	std::string user_created;
	std::string user_is_bot;
	std::string user_has_nitro;
	std::string user_joined_server;
	std::string user_premium;
	std::string user_mention;
	std::string title;
	std::string response;

	dpp::command_interaction cmd_data;
	dpp::snowflake user;
	dpp::user_identified user_t;
	dpp::guild_member guild_user_t;

	cmd_data = event.command.get_command_interaction();
	user = cmd_data.get_value<dpp::snowflake>(0);
	user_t = bot.user_get_sync(user);
	guild_user_t = bot.guild_get_member_sync(event.command.get_guild().id, user);

	event.thinking();

	user_username = user_t.format_username();
	user_avatar = user_t.get_avatar_url();
	user_created = fmt::format("<t:{}:D>", std::to_string((int) user_t.get_creation_time()));

	user_is_bot = (user_t.is_bot()) ? dpp::unicode_emoji::white_check_mark : dpp::unicode_emoji::no_entry_sign;
	user_has_nitro = (user_t.has_nitro_classic() || user_t.has_nitro_full() || user_t.has_nitro_basic())
					 ? dpp::unicode_emoji::white_check_mark : dpp::unicode_emoji::no_entry_sign;
	user_joined_server = fmt::format("<t:{}:D>", std::to_string((int) guild_user_t.joined_at));
	user_premium = ((int) guild_user_t.premium_since > 0) ? fmt::format("<t:{}:D>", std::to_string(
			(int) guild_user_t.premium_since)) : dpp::unicode_emoji::no_entry_sign;

	user_mention = user_t.get_mention();
	title = "Who is @" + user_username;

	const int has_nitro = user_t.has_nitro_full() || user_t.has_nitro_basic() || user_t.has_nitro_classic();
	const int has_boost = guild_user_t.premium_since > 0 ? 1 : 0;

	if (user == "nulzo") {
		prompt = "Make up really good things about a user named nulzo! They are so awesome!";
	} else {
		prompt = fmt::format("Write a very short description of some shit about a loser who goes by the name {}.", user_username);
	}
	if (has_nitro == 0) {
		prompt += " Mention that the person doesn't have Nitro, which is lame.";
	}
	if (has_boost == 0) {
		prompt += " Mention that the person doesn't boost the server, which is very lame.";
	}

	response = open_ai_api(prompt, "300", "gpt-3.5-turbo-1106");

	dpp::embed embed = dpp::embed()
			.set_color(Cringe::CringeColor::CringeBlack)
			.set_title(title)
			.set_description(response)
			.set_thumbnail(user_avatar)
			.add_field("Username", user_mention, true)
			.add_field("Account Created", user_created, true)
			.add_field("Joined Server", user_joined_server, true)
			.add_field("Booster", user_premium, true)
			.add_field("Nitro", user_has_nitro, true)
			.add_field("Bot", user_is_bot, true)
			.set_timestamp(0);

	dpp::message msg(event.command.channel_id, embed);

	event.edit_original_response(msg);
}