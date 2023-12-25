#include "commands.h"

dpp::slashcommand message_declaration() {
    constexpr int64_t min_val{1};
    constexpr int64_t max_val{100};
    return dpp::slashcommand()
            .set_name("message")
            .set_description("Bruh I don't even know")
            .add_option(
                    dpp::command_option(dpp::co_integer, "quantity", "Quantity of messages to get. Max - 100.")
                            .set_min_value(min_val)
                            .set_max_value(max_val)
            );
}

dpp::slashcommand ethan_declaration() {
    return dpp::slashcommand()
            .set_name("ethan")
            .set_description("...");
}

void message_command(dpp::cluster &bot, const dpp::slashcommand_t &event) {

    int64_t limit = std::get<int64_t>(event.get_parameter("quantity"));

    /* get messages using ID of the channel the command was issued in */
    bot.messages_get(event.command.channel_id, 0, 0, 0, limit, [event](const dpp::confirmation_callback_t& callback) {
        if (callback.is_error()) { /* catching an error to log it */
            std::cout << callback.get_error().message << std::endl;
            return;
        }

        auto messages = callback.get<dpp::message_map>();
        /* std::get<dpp::message_map>(callback.value) would give the same result */

        std::string contents;
        for (const auto& x : messages) { /* here we iterate through the dpp::message_map we got from callback... */
            contents += x.second.content + '\n'; /* ...where x.first is ID of the current message and x.second is the message itself. */
        }

        event.reply(contents); /* we will see all those messages we got, united as one! */
    });
}

void ethan_command(dpp::cluster &bot, const dpp::slashcommand_t &event) {
    auto target_user = bot.guild_get_member_sync(1069835760859107368, 411399698679595008);
    target_user.set_nickname("New Nickname");
    bot.guild_edit_member(target_user);
    event.reply(fmt::format("Hey, {}, checkout your username!", target_user.get_mention()));
}