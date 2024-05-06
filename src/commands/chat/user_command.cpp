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

#include "commands/chat/user_command.h"
#include "utils/embed/cringe_embed.h"
#include "utils/http/cringe_api.h"

dpp::slashcommand user_declaration() {
	return dpp::slashcommand()
			.set_name("user")
			.set_description("Get general information about a user")
			.add_option(dpp::command_option(dpp::co_user, "username", "The user to view", true));
}

void user_command(dpp::cluster &bot, const dpp::slashcommand_t &event) {
	event.thinking();
	std::string prompt;
	dpp::command_interaction cmd_data = event.command.get_command_interaction();
	dpp::snowflake user = cmd_data.get_value<dpp::snowflake>(0);
	dpp::user_identified user_t = bot.user_get_sync(user);
	dpp::guild_member guild_user_t = bot.guild_get_member_sync(event.command.get_guild().id, user);
	std::string user_username = user_t.format_username();
	std::string user_avatar = user_t.get_avatar_url();
	std::string user_created = fmt::format("<t:{}:D>", std::to_string((int) user_t.get_creation_time()));
	std::string user_is_bot = (user_t.is_bot()) ? dpp::unicode_emoji::white_check_mark : dpp::unicode_emoji::no_entry_sign;
	std::string user_has_nitro = (user_t.has_nitro_classic() || user_t.has_nitro_full() || user_t.has_nitro_basic())
					 ? dpp::unicode_emoji::white_check_mark : dpp::unicode_emoji::no_entry_sign;
	std::string user_joined_server = fmt::format("<t:{}:D>", std::to_string((int) guild_user_t.joined_at));
	std::string user_premium = ((int) guild_user_t.premium_since > 0) ? fmt::format("<t:{}:D>", std::to_string(
			(int) guild_user_t.premium_since)) : dpp::unicode_emoji::no_entry_sign;
	std::string user_mention = user_t.get_mention();
	std::string title = "Who is @" + user_username;
	const int has_nitro = user_t.has_nitro_full() || user_t.has_nitro_basic() || user_t.has_nitro_classic();
	const int has_boost = guild_user_t.premium_since > 0 ? 1 : 0;
	if (user_t.global_name == "nulzo") {
		prompt = "Make up really good things about a user named nulzo! They are so awesome!";
	} else {
		prompt = fmt::format("Write a very short description of some shit about a loser who goes by the name {}.", user_username);
	}
	if (has_nitro == 0) {
		prompt += " Mention that the person doesn't have Nitro, which is lame.";
	}
	if (has_boost == 0) {
		prompt += " Include that this user is a stupid idiot.";
	}
	// std::string response = get_openai_response(prompt, "300", "gpt-3.5-turbo-1106");
	std::string response = cringe_chat(prompt, "cringe");
	dpp::embed embed = info_embed(title, response, user_avatar, user_mention, user_created, user_joined_server, user_premium, user_has_nitro, user_is_bot);
	dpp::message msg(event.command.channel_id, embed);
	event.edit_original_response(msg);
}