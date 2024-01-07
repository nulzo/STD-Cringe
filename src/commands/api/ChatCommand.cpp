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

#include "commands/api/api.h"
#include "utils/util.h"
#include "utils/embed.h"

dpp::slashcommand chat_declaration() {
	return dpp::slashcommand()
			.set_name("chat")
			.set_description("Chat with cringe")
			.add_option(dpp::command_option(dpp::co_string, "prompt", "What to ask cringe", true));
}

void chat_command(dpp::cluster &bot, const dpp::slashcommand_t &event) {
	event.thinking(true);
	std::string prompt = std::get<std::string>(event.get_parameter("prompt"));
	std::string data;
	std::vector<std::string> result;
	std::string response = open_ai_api(prompt, "750", "gpt-4");
	std::istringstream stream(response);
	// Loop over the string, splitting at 2k
	if (response.length() >= 2000) {
//		while (std::getline(stream, data, '\n')) {
//			result.push_back(data);
//		}
		std::vector<std::string> chunks;

		for (size_t i = 0; i < response.length(); i += 2000) {
			chunks.push_back(response.substr(i, 2000));
		}

		for (const auto &substring: chunks) {
			dpp::message msg(event.command.channel.id, substring);
			bot.message_create(msg);
		}
	} else {
		dpp::message msg(event.command.channel_id, response);
		std::cout << "\n" << event.command.channel.id << "\n";
		bot.message_create(msg);
	}
	dpp::message msg(event.command.channel.id, "Your prompt was sent!");
	event.edit_original_response(msg);
}
