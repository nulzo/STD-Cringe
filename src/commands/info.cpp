#include "commands.h"

dpp::slashcommand info_declaration() {
    return dpp::slashcommand()
            .set_name("info")
            .set_description("General information about the bot");
}

void info_command(dpp::cluster& bot, const dpp::slashcommand_t& event) {
    dpp::embed embed = dpp::embed()
            .set_color(color::CringeOrange)
            .set_title("std::cringe bot")
            .set_description("Probably the **most based** discord bot you've *ever* seen in your life. Python? Never heard of it. Javascript? That shit ain't nothing to me, man. This bot right here was coded in pure C++, fuck you know about it? Put simply - this bot runs laps around your run-of-the-mill shitty bot coded in some rinky-dink language. Get real, get hard, get **cringe**.")
            .set_thumbnail("https://cdn.discordapp.com/attachments/994332421753425951/1187180553821225000/Black_And_White_Aesthetic_Minimalist_Modern_Simple_Typography_Coconut_Cosmetics_Logo.png?ex=6595f30c&is=65837e0c&hm=546158d3750abdbaf01a83d3800061494ceda3f0341b08a01dcc8becf29aa567&")
            .add_field(
                    "Creator",
                    bot.user_get_sync(933796468731568191).get_mention(),
                    true
            )
            .add_field(
                    "Created",
                    fmt::format("<t:{}:D>", std::to_string((int)bot.user_get_sync(1186860332845629511).get_creation_time())),
                    true
            )
            .add_field(
                    "Active In",
                    std::to_string(dpp::get_guild_cache()->count()) + " servers",
                    true
            )
            .set_footer(
                    dpp::embed_footer()
                            .set_text("v0.0.1")

            );

    /* Create a message with the content as our new embed. */
    dpp::message msg(event.command.channel_id, embed);

    /* Reply to the user with the message, containing our embed. */
    event.reply(msg);
}