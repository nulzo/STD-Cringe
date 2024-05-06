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

#include "commands/voice/join_command.h"

#include <fmt/format.h>

#include <algorithm>

#include "utils/embed/cringe_embed.h"

dpp::slashcommand join_declaration() {
    return dpp::slashcommand()
        .set_name("join")
        .set_description("Join the voice channel you are in")
        .add_option(dpp::command_option(
            dpp::co_channel, "channel",
            "The VC for bot to join (defaults to your current channel)",
            false));
}

void join_command(dpp::cluster &bot, const dpp::slashcommand_t &event) {
    dpp::guild *guild;
    dpp::snowflake parameter;
    dpp::channel requested_channel;
    event.thinking(true);
    // Get the guild of issued command
    guild = dpp::find_guild(event.command.guild_id);
    // Set flag if the channel has been found
    auto p = event.get_parameter("channel");
    if (p.index() != 0) {
        parameter = std::get<dpp::snowflake>(event.get_parameter("channel"));
        // Get the param (if any) and find voice channel id
        requested_channel = bot.channel_get_sync(parameter);
        if (!requested_channel.is_voice_channel()) {
            std::string error_reason = event.command.usr.get_mention() +
                                       " tried to invite cringe to VC, but "
                                       "provided a non-voice channel.";
            dpp::message message(event.command.channel_id,
                                 cringe_error_embed(error_reason).embed);
            event.edit_original_response(message);
            return;
        }

        auto channel = event.from->get_voice(event.command.guild_id);

        if (channel) {
            auto users_vc = guild->voice_members.find(bot.me.id);
            if (users_vc != guild->voice_members.end())
                if (channel->channel_id == users_vc->second.channel_id) {
                    // Issue warning that cringe must be called to a channel
                    // that it is not already in
                    std::string warning_reason =
                        event.command.usr.get_mention() +
                        " tried to invite cringe to "
                        "VC, but it is already there.";
                    dpp::message message(
                        event.command.channel_id,
                        cringe_warning_embed(warning_reason).embed);
                    event.edit_original_response(message);
                    return;
                }
            event.from->disconnect_voice(event.command.guild_id);
        }
        event.from->connect_voice(guild->id, requested_channel.id);
        std::string success_reason = event.command.usr.get_mention() +
                                     " asked cringe to join " +
                                     requested_channel.get_mention();
        dpp::message message(event.command.channel_id,
                             cringe_success_embed(success_reason).embed);
        event.edit_original_response(message);
        return;
    }

    // Get the voice channel that cringe is currently in from this server (null
    // if cringe not in vc)
    auto channel = event.from->get_voice(event.command.guild_id);

    // If cringe currently in VC, see which channel cringe is in
    if (channel) {
        // Find the channel that the issuing user is in
        auto users_vc =
            guild->voice_members.find(event.command.get_issuing_user().id);
        // Check if issuing user is trying to add cringe to channel it is
        // already in
        if (users_vc != guild->voice_members.end() &&
            channel->channel_id == users_vc->second.channel_id) {
            // Issue warning that cringe must be called to a channel that it is
            // not already in
            std::string warning_reason =
                event.command.usr.get_mention() +
                " tried to invite cringe to VC, but it is already there.";
            dpp::message message(event.command.channel_id,
                                 cringe_warning_embed(warning_reason).embed);
            event.edit_original_response(message);
            return;
        }
        // At this point, cringe is being called into a channel that it is
        // currently not in (although it is in another channel)
        event.from->disconnect_voice(event.command.guild_id);
    }

    // Check to see if the issuing user is not in a voice channel and issue
    // error if they did not provide an option of where to go
    if (!guild->connect_member_voice(event.command.get_issuing_user().id)) {
        std::string error_reason =
            "You must be in a VC, or specify a channel if "
            "you wish to invite cringe to VC.";
        dpp::message message(event.command.channel_id,
                             cringe_error_embed(error_reason).embed);
        event.edit_original_response(message);
        return;
    }

    // At this point, cringe has successfully joined the VC, and a success
    // message can be issued
    std::string success_reason =
        event.command.usr.get_mention() + " asked cringe to join.";
    dpp::message message(event.command.channel_id,
                         cringe_success_embed(success_reason).embed);
    event.edit_original_response(message);
}