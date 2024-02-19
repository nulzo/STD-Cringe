/*
 * MIT License
 *
 * Copyright (c) 2024 @nulzo
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

#include "commands/api/DescribeCommand.h"
#include "utils/util.h"
#include "utils/embed.h"

dpp::slashcommand describe_declaration() {
	return dpp::slashcommand()
			.set_name("describe")
			.set_description("Describe an image")
			.add_option(dpp::command_option(dpp::co_attachment, "file", "Select an image"));
}

void describe_command(dpp::cluster &bot, const dpp::slashcommand_t &event) {
	// Set the command to thinking and set it to ephemeral
	event.thinking();
	std::string response;
	dpp::embed embed;
	/* Get the file id from the parameter attachment. */
	dpp::snowflake file_id = std::get<dpp::snowflake>(event.get_parameter("file"));
	/* Get the attachment that the user inputted from the file id. */
	dpp::attachment att = event.command.get_resolved_attachment(file_id);
	response = get_ollama_describe(att.url);
	embed.set_color(Cringe::CringeColor::CringeOrange)
			.set_title("Cringe Describer")
			.set_thumbnail(Cringe::CringeIcon::SnailIcon)
			.set_description(response)
			.set_image(att.url)
			.set_timestamp(time(nullptr))
			.set_footer(fmt::format("Requested by: {}", event.command.usr.global_name), event.command.usr.get_avatar_url());
	/* Reply with the file as a URL. */
	dpp::message msg(event.command.channel_id, embed);
	event.edit_original_response(msg);
}
