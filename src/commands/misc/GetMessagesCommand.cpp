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

#include "commands/misc/misc.h"
#include "utils/cringe.h"

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

void message_command(dpp::cluster &bot, const dpp::slashcommand_t &event) {
	int64_t limit = std::get<int64_t>(event.get_parameter("quantity"));
	bot.messages_get(
			event.command.channel_id, 0, 0, 0, limit,
			[event](const dpp::confirmation_callback_t &callback) {
				if (callback.is_error()) {
					std::cout << callback.get_error().message << std::endl;
					return;
				}
				auto messages = callback.get<dpp::message_map>();
				std::string contents;
				for (const auto &x: messages) {
					contents += x.second.content + '\n';
				}
				event.reply(contents);
			});
}