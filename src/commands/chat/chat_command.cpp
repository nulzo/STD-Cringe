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

#include "commands/chat/chat_command.h"
#include "utils/misc/cringe_helpers.h"
#include "utils/embed/cringe_embed.h"
#include "connectors/cringe_database.h"
#include "utils/http/cringe_api.h"

dpp::slashcommand chat_declaration() {
	return dpp::slashcommand()
			.set_name("chat")
			.set_description("Chat with a model")
			.add_option(dpp::command_option(dpp::co_string, "model", "The model to interact with", true)
								.add_choice(dpp::command_option_choice("Cringe", std::string("cringe")))
								.add_choice(dpp::command_option_choice("Klim", std::string("klim")))
								.add_choice(dpp::command_option_choice("Ethan", std::string("ethan")))
								.add_choice(dpp::command_option_choice("Joeman", std::string("joeman"))))
			.add_option(dpp::command_option(dpp::co_string, "prompt", "What to ask the model", true));
}

void chat_command(dpp::cluster &bot, const dpp::slashcommand_t &event) {
	event.thinking(true);
	std::string channel = get_env("CRINGE_CHAT_CHANNEL");
	// Query the ollama endpoint with the prompt the user provided
	std::string prompt = std::get<std::string>(event.get_parameter("prompt"));
	std::string model = std::get<std::string>(event.get_parameter("model"));
	json r = cringe_chat(prompt, model);
	if(!r["error"].empty()) {
		std::cout << "\nERROR!\n" << std::endl;
		// An error has occurred!
	}
	std::string response = r["response"];
	// Access the database and insert the values
	CringeDB cringe_db(get_env("CRINGE_DATABASE"));
	cringe_db.execute("CREATE TABLE IF NOT EXISTS CHAT (id INTEGER PRIMARY KEY AUTOINCREMENT, model TEXT, issuer TEXT, prompt TEXT, response TEXT);");
	std::vector<std::string> params = {model, event.command.usr.username, prompt, response};
	cringe_db.execute("INSERT INTO CHAT (model,issuer,prompt,response) VALUES (?, ?, ?, ?);", params);
	CringeEmbed cringe_embed;
	cringe_embed.setTitle("Cringe Chat").setHelp(fmt::format("ask {} a question with /chat!", model));
	cringe_embed.setFields({{fmt::format("{} asked", event.command.usr.username), prompt, "false"},
													{fmt::format("{} responded", model), response, "false"}});
	// Issue the replies
	dpp::message cringe_response(channel, cringe_embed.embed);
	bot.message_create(cringe_response);
	dpp::message ephemeral_reply(event.command.channel.id, fmt::format("Your chat has been responded to in {}!", bot.channel_get_sync(channel).get_mention()));
	event.edit_original_response(ephemeral_reply);
}
