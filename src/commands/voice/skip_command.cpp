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

#include "commands/voice/skip_command.h"

#include <fmt/format.h>

#include "commands/voice/play_command.h"
#include "utils/embed/cringe_embed.h"
#include "utils/misc/cringe.h"

dpp::slashcommand skip_declaration() {
    return dpp::slashcommand().set_name("skip").set_description(
        "Skip the song that is currently playing");
}

void skip_command(dpp::cluster &bot, const dpp::slashcommand_t &event,
                  CringeQueue &queue) {
    std::string embed_reason;
    dpp::embed embed;
    event.thinking(true);
    /* Get the voice channel the bot is in, in this current guild. */
    dpp::voiceconn *v = event.from->get_voice(event.command.guild_id);

    if (!v || !v->voiceclient || !v->voiceclient->is_ready()) {
        std::string error_reason = "Bot was unable to join the voice channel "
                                   "due to some unknown reason.";
        dpp::message message(event.command.channel_id,
                             cringe_error_embed(error_reason).embed);
        event.edit_original_response(message);
        return;
    }

    if (queue.is_empty() && !v->voiceclient->is_playing()) {
        std::string error_reason = "There is no song to skip.";
        dpp::message message(event.command.channel_id,
                             cringe_error_embed(error_reason).embed);
        event.edit_original_response(message);
        return;
    }

    if (v->voiceclient->is_playing()) {
        v->voiceclient->skip_to_next_marker();
        std::string success_reason =
            "Successfully skipped song. Playing next in queue.";
        dpp::message message(event.command.channel_id,
                             cringe_success_embed(success_reason).embed);
        event.edit_original_response(message);
        return;
    }

    CringeSong s = queue.dequeue();
    v->voiceclient->skip_to_next_marker();
    play_callback(bot, s);
    std::string success_reason =
        "Successfully skipped song. Playing next in queue.";
    dpp::message message(event.command.channel_id,
                         cringe_success_embed(success_reason).embed);
    event.edit_original_response(message);
}
