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

#include "commands/voice/play_command.h"
#include <fmt/format.h>
#include <algorithm>
#include "dpp/dpp.h"
#include "utils/embed/cringe_embed.h"
#include "utils/audio/cringe_audio.h"
#include "utils/audio/cringe_youtube.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
#include <opus/opus.h>
}

dpp::slashcommand play_declaration() {
    return dpp::slashcommand()
        .set_name("play")
        .set_description("Play a song in the voice channel you are in")
        .add_option(dpp::command_option(
            dpp::co_string, "song",
            "Song you wish to stream (either query or URL)", true))
        .add_option(
            dpp::command_option(dpp::co_string, "filter",
                                "Filter to apply to the song", false)
                .add_choice(dpp::command_option_choice(
                    "Bass Boosted", std::string("bassboost")))
                .add_choice(dpp::command_option_choice(
                    "Vaporwave", std::string("vaporwave")))
                .add_choice(dpp::command_option_choice(
                    "Nightcore", std::string("nightcore")))
                .add_choice(dpp::command_option_choice("In The Bathroom",
                                                       std::string("bathroom")))
                .add_choice(
                    dpp::command_option_choice("Lofi", std::string("lofi")))
                .add_choice(
                    dpp::command_option_choice("DIM", std::string("dim")))
                .add_choice(dpp::command_option_choice("Expander",
                                                       std::string("expand"))));
}

void play_command(CringeBot &cringe, const dpp::slashcommand_t &event) {
    event.thinking(true);
	CringeEmbed embed;
	CringeAudioStreamer cringe_audio;
    dpp::voiceconn *voice = event.from->get_voice(event.command.guild_id);
	dpp::guild *guild = dpp::find_guild(event.command.guild_id);

	// If the voice channel was invalid, or there is an issue with it
    if ((voice == nullptr) || (voice->voiceclient == nullptr) || !voice->voiceclient->is_ready()) {
		bool response = guild->connect_member_voice(event.command.get_issuing_user().id);
		if(!response) {
			std::string error_reason = "You must be in a voice channel to stream audio";
			dpp::message message(event.command.channel_id, cringe_error_embed(error_reason).embed);
			event.edit_original_response(message);
			return;
		}
		bool is_ready = false;
		cringe.cluster.on_voice_ready([&is_ready](const dpp::voice_ready_t &ready) {
			is_ready = true;
		});
		while(!is_ready);
		voice = event.from->get_voice(event.command.guild_id);
    }

	const dpp::channel channel = event.command.channel;
    std::string request = std::get<std::string>(event.get_parameter("song"));
	std::string filter = event.get_parameter("filter").index() > 0 ? std::get<std::string>(event.get_parameter("filter")) : "";

    // Check if queue is not empty (or if song is currently playing)
    if (!cringe.queue.is_empty() || voice->voiceclient->is_playing()) {
		cringe.queue.enqueue(request, filter);
        dpp::message message(event.command.channel_id, embed.embed);
        event.edit_original_response(message);
        return;
    }

	cringe_audio.stream(voice, request, filter);

	embed.setTitle("Now Streaming").setDescription("some song ;p");
    dpp::message message(channel.id, embed.embed);
    cringe.cluster.message_create(message);
    dpp::message msg(event.command.channel_id, "added song");
    event.edit_original_response(msg);
}
