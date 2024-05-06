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
#include "utils/http/cringe_api.h"
#include "utils/misc/cringe_helpers.h"

auto code_declaration() -> dpp::slashcommand {
    return dpp::slashcommand()
        .set_name("code")
        .set_description("Ask a coding question")
        .add_option(dpp::command_option(dpp::co_string, "prompt",
                                        "Coding question to ask", true));
}

void code_command(dpp::cluster &bot, const dpp::slashcommand_t &event) {
    event.thinking(true);
    std::string channel = get_env("CRINGE_CODE_CHANNEL");
    std::string prompt = std::get<std::string>(event.get_parameter("prompt"));
    CringeDB cringe_db(get_env("CRINGE_DATABASE"));

    std::vector<std::vector<std::string>> rows = cringe_db.query(fmt::format(
        "SELECT * FROM chat WHERE issuer = '{}' AND model = '{}' AND "
        "created >= datetime('now', '-5 minutes');",
        event.command.usr.username, "code"));
    // db goes [model,issuer,prompt,response]
    std::string init_string = "[";
    for (const auto &row : rows) {
        std::string newstring = fmt::format(R"({{ " role ": " user ", " content ": "{}" }}, {{ " role ": " assistant ", " content ": " {} " }},)",row[3], row[4]);
        init_string += newstring;
    }
    init_string += fmt::format(R"({{ " role ": " user ", " content ": " {} " }}])", prompt);
    json res = cringe_chat(prompt, "code");
    if (!res["error"].empty()) {
        std::cout << "\nERROR!\n" << std::endl;
        // An error has occurred!
    }
    std::string response = res["response"];
    // Access the database and insert the values

    cringe_db.execute("CREATE TABLE IF NOT EXISTS CHAT (id INTEGER PRIMARY KEY "
                      "AUTOINCREMENT, "
                      "model TEXT, issuer TEXT, prompt TEXT, response TEXT, "
                      "created TIMESTAMP "
                      "DEFAULT CURRENT_TIMESTAMP);");
    std::vector<std::string> params = {"code", event.command.usr.username,
                                       prompt, response};
    cringe_db.execute(
        "INSERT INTO CHAT (model,issuer,prompt,response) VALUES (?, ?, ?, ?);",
        params);

    std::string part;
    int chunk_max = 1024;
    if (response.length() >= chunk_max) {
        for (size_t i = 0; i < response.length(); i += chunk_max) {
            std::string chunk = response.substr(i, chunk_max);
            part = (i == 0)
                       ? fmt::format("**{} asked**\n{}\n**cringe replied**\n{}",
                                     event.command.usr.username, prompt, chunk)
                       : chunk;
            dpp::message cringe_reply(channel, part);
            bot.message_create(cringe_reply);
        }
    } else {
        std::string content =
            fmt::format("**{} asked**\n{}\n**cringe replied**\n{}",
                        event.command.usr.username, prompt, response);
        // Issue the replies
        dpp::message cringe_reply(channel, content);
        bot.message_create(cringe_reply);
    }

    dpp::message ephemeral_reply(
        event.command.channel.id,
        fmt::format("Your chat has been responded to in {}!",
                    bot.channel_get_sync(channel).get_mention()));
    event.edit_original_response(ephemeral_reply);
}
