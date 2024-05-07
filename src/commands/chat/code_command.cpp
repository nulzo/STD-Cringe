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
#include "connectors/cringe_database.h"
#include "utils/embed/cringe_embed.h"

auto code_declaration() -> dpp::slashcommand {
    return dpp::slashcommand()
        .set_name("code")
        .set_description("Ask a coding question")
        .add_option(dpp::command_option(dpp::co_string, "prompt",
                                        "Coding question to ask", true));
}

void code_command(CringeBot &cringe, const dpp::slashcommand_t &event) {
    event.thinking(true);
    dpp::channel channel = event.command.channel;
    std::string prompt = std::get<std::string>(event.get_parameter("prompt"));
    json ollama_response = cringe.ollama.chat(prompt, "code");
    std::string response = ollama_response["response"];
    std::string part;
    int chunk_max = 1024;
    if (response.length() >= chunk_max) {
        for (size_t i = 0; i < response.length(); i += chunk_max) {
            std::string chunk = response.substr(i, chunk_max);
            part = (i == 0)
                       ? fmt::format("**{} asked**\n{}\n**cringe replied**\n{}",
                                     event.command.usr.username, prompt, chunk)
                       : chunk;
            dpp::message cringe_reply(channel.id, part);
            cringe.cluster.message_create(cringe_reply);
        }
    } else {
        std::string content = fmt::format("**{} asked**\n{}\n**cringe replied**\n{}", event.command.usr.username, prompt, response);
        dpp::message cringe_reply(channel.id, content);
        cringe.cluster.message_create(cringe_reply);
    }
    dpp::message ephemeral_reply(channel.id, fmt::format("Your chat has been responded to in {}!", channel.get_mention()));
    event.edit_original_response(ephemeral_reply);
}
