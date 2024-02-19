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

#include "commands/api/ImageCommand.h"
#include "utils/util.h"
#include "utils/embed.h"
#include "utils/base64.h"


dpp::slashcommand image_declaration() {
	return dpp::slashcommand()
			.set_name("imagine")
			.set_description("Have cringe generate an image")
			.add_option(dpp::command_option(dpp::co_string, "prompt", "Prompt for image", true));
}

void image_command(dpp::cluster &bot, const dpp::slashcommand_t &event) {
	std::string prompt = std::get<std::string>(event.get_parameter("prompt"));
	event.thinking(true);
	std::string channel;
	get_env("CRINGE_TESTING_CHANNEL", channel);
	std::string image = get_image(prompt);
	std::string binaryData = base64_decode(image);
	dpp::message message(channel, "");
	message.add_file("imagine.jpg", binaryData);
	dpp::embed embed;
	std::string response = fmt::format("{} - {}", prompt, event.command.usr.get_mention());
	embed.set_color(Cringe::CringeColor::CringeOrange)
			.set_title("Cringe Imagination")
			.set_thumbnail(Cringe::CringeIcon::SnailIcon)
			.set_description(response)
			.set_image("attachment://imagine.jpg")
			.set_timestamp(time(nullptr))
			.set_footer(fmt::format("Imagined by: {}", event.command.usr.global_name), event.command.usr.get_avatar_url());
	message.add_embed(embed);
	bot.message_create(message);
	dpp::message ephemeral_reply(event.command.channel.id, fmt::format("cringe has responded to your chat in {}!", bot.channel_get_sync(channel).get_mention()));
	event.edit_original_response(ephemeral_reply);
}