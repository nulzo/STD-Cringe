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
	std::string prompt = std::get<std::string>(event.get_parameter("prompt"));
	event.thinking();
	std::string response = open_ai_api(prompt, "250", "gpt-3.5-turbo-1106");
	event.edit_original_response(response);
}