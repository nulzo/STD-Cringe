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
#include "fmt/format.h"
#include "commands/chat/info_command.h"

dpp::slashcommand info_declaration() {
    return dpp::slashcommand().set_name("info").set_description(
        "General information about the bot");
}

void info_command(CringeBot &cringe, const dpp::slashcommand_t &event) {
    dpp::embed embed =
        dpp::embed()
            .set_color(Cringe::CringeColor::CringePrimary)
            .set_title("std::cringe bot")
            .set_description(
                "Probably the **most based** discord bot you've *ever* seen "
                "in "
                "your life. Python? Never heard of it. Javascript? That shit "
                "ain't nothing to me, man. This bot right here was coded in "
                "pure "
                "C++, fuck you know about it? Put simply - this bot runs laps "
                "around your run-of-the-mill shitty bot coded in some "
                "rinky-dink "
                "language. Get real, get hard, get **cringe**.")
            .set_thumbnail(
                "https://cdn.discordapp.com/avatars/1186860332845629511/"
                "2b20f3636a5bd288bca2eb197badf556.png")
            .add_field("Creator",
                       cringe.cluster.user_get_sync(933796468731568191).get_mention(),
                       true)
            .add_field(
                "Created",
                fmt::format(
                    "<t:{}:D>",
                    std::to_string((int)cringe.cluster.user_get_sync(1186860332845629511)
                                       .get_creation_time())),
                true)
            .add_field("Active In",
                       std::to_string(dpp::get_guild_cache()->count()) +
                           " servers",
                       true)
            .set_footer(dpp::embed_footer().set_text("v0.0.1")

            );

    /* Create a message with the content as our new embed. */
    dpp::message msg(event.command.channel_id, embed);

    /* Reply to the user with the message, containing our embed. */
    event.reply(msg);
}