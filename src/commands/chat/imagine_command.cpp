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

#include "commands/chat/imagine_command.h"
#include "utils/embed/cringe_embed.h"
#include "utils/misc/cringe_64.h"

auto image_declaration() -> dpp::slashcommand {
    return dpp::slashcommand()
        .set_name("imagine")
        .set_description("Have cringe generate an image")
        .add_option(dpp::command_option(dpp::co_string, "style",
                                        "The style of the image", true)
                        .add_choice(dpp::command_option_choice(
                            "Realistic", std::string("realistic")))
                        .add_choice(dpp::command_option_choice(
                            "Anime", std::string("anime")))
                        .add_choice(dpp::command_option_choice(
                            "Creative", std::string("creative"))))
        .add_option(dpp::command_option(dpp::co_string, "prompt",
                                        "Prompt for image", true));
}

void image_command(CringeBot &cringe, const dpp::slashcommand_t &event) {
    event.thinking(true);
    std::string prompt = std::get<std::string>(event.get_parameter("prompt"));
    std::string style = std::get<std::string>(event.get_parameter("style"));
    dpp::channel channel = event.command.channel;
    json ollama_response = cringe.ollama.chat(prompt, style);
    std::string image = ollama_response["response"];
    std::string binaryData = cringe64_decode(image);
    dpp::message message(channel.id, "");
    message.add_file("imagine.jpg", binaryData);
    CringeEmbed cringe_embed;
    cringe_embed.setTitle("Cringe Imagine")
        .setHelp("generate an image with /imagine!")
        .setImage(fmt::format("attachment://{}", "imagine.jpg"))
        .setDescription(prompt);
    message.add_embed(cringe_embed.embed);
    cringe.cluster.message_create(message);
    dpp::message ephemeral_reply(
        event.command.channel.id,
        fmt::format("cringe has responded to your chat in {}!", cringe.cluster.channel_get_sync(channel.id).get_mention()));
    event.edit_original_response(ephemeral_reply);
}
