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

#include "commands/api/CodeCommand.h"
#include "utils/util.h"
#include "utils/embed.h"

dpp::slashcommand code_declaration() {
	return dpp::slashcommand()
			.set_name("code")
			.set_description("Have cringe answer coding related problems")
			.add_option(dpp::command_option(dpp::co_string, "prompt", "Coding question", true));
}

void code_command(dpp::cluster &bot, const dpp::slashcommand_t &event) {
	// Set the command to thinking and set it to ephemeral
	event.thinking(true);
	std::string channel = get_env("CRINGE_CHAT_CHANNEL");
	// Query the ollama endpoint with the prompt the user provided
	std::string prompt = std::get<std::string>(event.get_parameter("prompt"));
	std::string response = get_ollama_code(prompt);
	// Make the replies
	if(response.length() >= 5000) {
        for (size_t i = 0; i < response.length(); i += 5000) {
            std::string chunk = response.substr(i, 5000);
            dpp::embed embed = chat_embed(prompt, chunk, event);
            dpp::message cringe_response(channel, embed);
            bot.message_create(cringe_response);
        }
    }
	else {
		dpp::embed embed = chat_embed(prompt, response, event);
		dpp::message cringe_response(channel, embed);
		// Issue the replies
		bot.message_create(cringe_response);
	}
	dpp::message ephemeral_reply(event.command.channel.id, fmt::format("Cringe has responded to your chat in {}!", bot.channel_get_sync(channel).get_mention()));
	event.edit_original_response(ephemeral_reply);
}
