/*
 * MIT License
 *
 * Copyright (c) 2024 @nulzo
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

#include "commands/chat/describe_command.h"
#include "utils/embed/cringe_embed.h"

dpp::slashcommand describe_declaration() {
    return dpp::slashcommand()
        .set_name("describe")
        .set_description("Describe an image")
        .add_option(dpp::command_option(dpp::co_attachment, "image",
                                        "Select an image", true));
}

void describe_command(CringeBot &cringe, const dpp::slashcommand_t &event) {
    // Set the command to thinking and set it to ephemeral
    event.thinking(true);
    dpp::channel channel = event.command.channel;
    /* Get the file id from the parameter attachment. */
    dpp::snowflake image =
        std::get<dpp::snowflake>(event.get_parameter("image"));
    /* Get the attachment that the user inputted from the file id. */
    dpp::attachment attachment = event.command.get_resolved_attachment(image);
    json ollama_response = cringe.ollama.chat(attachment.url, "describe");
    std::string response = ollama_response["response"];
    CringeEmbed cringe_embed;
    cringe_embed.setTitle("Cringe Describe")
        .setHelp("describe an image with /describe!")
        .setDescription(response)
        .setImage(attachment.url);
    /* Reply with the file as a URL. */
    dpp::message message(channel.id, cringe_embed.embed);
    cringe.cluster.message_create(message);
    dpp::message ephemeral_reply(
        event.command.channel.id,
        fmt::format("cringe has described your image in {}!", channel.get_mention()));
    event.edit_original_response(ephemeral_reply);
}
