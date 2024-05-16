/*
 * MIT License
 *
 * Copyright (c) 2023 @nulzo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "commands/chat/chat_command.h"
#include "utils/database/cringe_database.h"
#include "utils/embed/cringe_embed.h"
#include "utils/http/cringe_ollama.h"

dpp::slashcommand chat_declaration(CringeBot &cringe) {
	dpp::slashcommand slashcommand = dpp::slashcommand().set_name("chat").set_description("Chat with a model");
	dpp::command_option option = dpp::command_option(dpp::co_string, "model", "The model to interact with", true);
	for(int i=0; i < std::min((int) cringe.models.size(), 25); i++) {
		option.add_choice(dpp::command_option_choice(cringe.models[i], std::string(cringe.models[i])));
	}
	return slashcommand.add_option(option).add_option(dpp::command_option(dpp::co_string, "prompt", "What to ask the model", true));
}

void chat_command(CringeBot &cringe, const dpp::slashcommand_t &event) {
    event.thinking(true);
    dpp::channel channel = event.command.channel;
    std::string prompt = std::get<std::string>(event.get_parameter("prompt"));
    std::string model = std::get<std::string>(event.get_parameter("model"));
    json ollama_response = cringe.ollama.chat(model, prompt);
    std::string response = ollama_response["response"];
    CringeEmbed cringe_embed;
    cringe_embed.setTitle("Cringe Chat").setHelp(fmt::format("ask {} a question with /chat!", model));
    cringe_embed.setFields(
        {{fmt::format("{} asked", event.command.usr.username), prompt, "false"},
         {fmt::format("{} responded", model), response, "false"}});
    dpp::message cringe_response(channel.id, cringe_embed.embed);
    cringe.cluster.message_create(cringe_response);
    dpp::message ephemeral_reply(event.command.channel.id, fmt::format("Your chat has been responded to in {}!", event.command.channel.get_mention()));
    event.edit_original_response(ephemeral_reply);
}
