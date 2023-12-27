//
// Created by Nolan Gregory on 12/26/23.
//

#include "commands/misc.h"
#include "utils/cringe.h"

dpp::slashcommand info_declaration() {
	return dpp::slashcommand()
			.set_name("info")
			.set_description("General information about the bot");
}

void info_command(dpp::cluster &bot, const dpp::slashcommand_t &event) {
	dpp::embed embed = dpp::embed()
			.set_color(Cringe::CringeColor::CringeOrange)
			.set_title("std::cringe bot")
			.set_description(
					"Probably the **most based** discord bot you've *ever* seen in your life. Python? Never heard of it. Javascript? That shit ain't nothing to me, man. This bot right here was coded in pure C++, fuck you know about it? Put simply - this bot runs laps around your run-of-the-mill shitty bot coded in some rinky-dink language. Get real, get hard, get **cringe**.")
			.set_thumbnail(
					"https://cdn.discordapp.com/attachments/994332421753425951/1187180553821225000/Black_And_White_Aesthetic_Minimalist_Modern_Simple_Typography_Coconut_Cosmetics_Logo.png?ex=6595f30c&is=65837e0c&hm=546158d3750abdbaf01a83d3800061494ceda3f0341b08a01dcc8becf29aa567&")
			.add_field(
					"Creator",
					bot.user_get_sync(933796468731568191).get_mention(),
					true
			)
			.add_field(
					"Created",
					fmt::format("<t:{}:D>",
								std::to_string((int) bot.user_get_sync(1186860332845629511).get_creation_time())),
					true
			)
			.add_field(
					"Active In",
					std::to_string(dpp::get_guild_cache()->count()) + " servers",
					true
			)
			.set_footer(
					dpp::embed_footer()
							.set_text("v0.0.1")

			);

	/* Create a message with the content as our new embed. */
	dpp::message msg(event.command.channel_id, embed);

	/* Reply to the user with the message, containing our embed. */
	event.reply(msg);
}

dpp::slashcommand message_declaration() {
	constexpr int64_t min_val{1};
	constexpr int64_t max_val{100};
	return dpp::slashcommand()
			.set_name("message")
			.set_description("Bruh I don't even know")
			.add_option(dpp::command_option(dpp::co_integer, "quantity",
											"Quantity of messages to get. Max - 100.")
								.set_min_value(min_val)
								.set_max_value(max_val));
}

dpp::slashcommand ethan_declaration() {
	return dpp::slashcommand().set_name("ethan").set_description("...");
}

void message_command(dpp::cluster &bot, const dpp::slashcommand_t &event) {
	int64_t limit = std::get<int64_t>(event.get_parameter("quantity"));

	/* get messages using ID of the channel the command was issued in */
	bot.messages_get(
			event.command.channel_id, 0, 0, 0, limit,
			[event](const dpp::confirmation_callback_t &callback) {
				if (callback.is_error()) { /* catching an error to log it */
					std::cout << callback.get_error().message << std::endl;
					return;
				}

				auto messages = callback.get<dpp::message_map>();
				/* std::get<dpp::message_map>(callback.value) would give the same result
				 */

				std::string contents;
				for (const auto &x: messages) {       /* here we iterate through the
             dpp::message_map we got       from callback... */
					contents += x.second.content + '\n'; /* ...where x.first is ID of the
      current message and x.second is the message itself. */
				}

				event.reply(contents); /* we will see all those messages we got, united
        as one! */
			});
}

void ethan_command(dpp::cluster &bot, const dpp::slashcommand_t &event) {
	auto target_user =
			bot.guild_get_member_sync(1069835760859107368, 411399698679595008);
	target_user.set_nickname("username");
	bot.guild_edit_member(target_user);
	event.reply(fmt::format("Hey, {}, checkout your username!",
							target_user.get_mention()));
}