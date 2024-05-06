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

#include "commands/chat/confession_command.h"

#include "utils/embed/cringe_embed.h"

dpp::slashcommand confession_declaration() {
    return dpp::slashcommand()
        .set_name("confess")
        .set_description("Confess to your sins (anonymously)")
        .add_option(dpp::command_option(dpp::co_string, "confession",
                                        "Your confession", true));
}

void confession_command(dpp::cluster &bot, const dpp::slashcommand_t &event) {
    std::string confession =
        std::get<std::string>(event.get_parameter("confession"));
    dpp::message response_message(
        dpp::message("Your confession has been sent...")
            .set_flags(dpp::m_ephemeral));
    event.reply(response_message);
    dpp::message message(event.command.channel_id,
                         confession_embed(confession));
    bot.message_create(message);
}