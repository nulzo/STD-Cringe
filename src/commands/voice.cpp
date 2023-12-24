#include "commands.h"

dpp::slashcommand join_declaration() {
    return dpp::slashcommand()
            .set_name("join")
            .set_description("Join the voice channel you are in");
}

dpp::slashcommand play_declaration() {
    return dpp::slashcommand()
            .set_name("play")
            .set_description("Play a song in the voice channel you are in");
}

void join_command(dpp::cluster &bot, const dpp::slashcommand_t &event) {

    /* Get the guild */
    dpp::guild *g = dpp::find_guild(event.command.guild_id);

    /* Get the voice channel that the bot is currently in from this server (will return nullptr if we're not in a voice channel!) */
    auto current_vc = event.from->get_voice(event.command.guild_id);

    bool join_vc = true;

    /* Are we in a voice channel? If so, let's see if we're in the right channel. */
    if (current_vc) {
        /* Find the channel id that the user is currently in */
        auto users_vc = g->voice_members.find(event.command.get_issuing_user().id);

        if (users_vc != g->voice_members.end() && current_vc->channel_id == users_vc->second.channel_id) {
            join_vc = false;
            /* We are on this voice channel, at this point we can send any audio instantly to vc
             */
        } else {
            /* We are on a different voice channel. We should leave it, then join the new one
             * by falling through to the join_vc branch below.
             */
            event.from->disconnect_voice(event.command.guild_id);
            join_vc = true;
        }
    }

    /* If we need to join a vc at all, join it here if join_vc == true */
    if (join_vc) {
        /* The user issuing the command is not on any voice channel, we can't do anything */
        if (!g->connect_member_voice(event.command.get_issuing_user().id)) {
            event.reply("You don't seem to be in a voice channel!");
            return;
        }

        /* Tell the user we joined their channel. */
        event.reply("Joined your channel!");
    } else {
        event.reply("Already in voice channel.");
    }
}

void play_command(dpp::cluster &bot, const dpp::slashcommand_t &event) {

    dpp::guild *g = dpp::find_guild(event.command.guild_id);

    /* Get the voice channel the bot is in, in this current guild. */
    dpp::voiceconn *v = event.from->get_voice(event.command.guild_id);

    /* If the voice channel was invalid, or there is an issue with it, then tell the user. */
    if (!v || !v->voiceclient || !v->voiceclient->is_ready()) {
        event.reply("There was an issue with getting the voice channel. Make sure I'm in a voice channel!");
        return;
    }
    event.reply("TEST");
}
