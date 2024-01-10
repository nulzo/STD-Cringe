//
// Created by Nolan Gregory on 1/8/24.
//

#include "commands/api/TalkCommand.h"

/*
 * MIT License
 *
 * Copyright (c) 2023 @nulzo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "commands/api/ImageCommand.h"
#include "utils/util.h"
#include "utils/embed.h"

dpp::slashcommand talk_declaration() {
	return dpp::slashcommand()
			.set_name("talk")
			.set_description("Have cringe talk in the VC")
			.add_option(dpp::command_option(dpp::co_string, "prompt", "Prompt for speech", true));
}

std::string talk_request(const std::string &prompt) {
	std::string res = get_tts_response(prompt);
	return res;
}

void talk_command(dpp::cluster &bot, const dpp::slashcommand_t &event) {
	std::string prompt = std::get<std::string>(event.get_parameter("prompt"));
	event.thinking(true);
	dpp::message msg(event.command.channel_id, "We are sorry, but this feature is currently disabled due to it sucking ass.");
	event.edit_original_response(msg);
//	std::string response = talk_request(prompt);
//	std::string formatted = fmt::format("ffmpeg -f mp3 -i pipe:0 -c:a pcm_s16le -f s16le pipe:1", "w");
//	auto pipe_in = popen("ffmpeg -i pipe:0 -f mp3 output.mp3", "w");
//	fwrite(response.c_str(), 1, response.size(), pipe_in);
//	pclose(pipe_in);
//	auto pipe = popen("ffmpeg -i output.mp3 -f mp3 pipe:1", "r");
//	dpp::voiceconn *voice = event.from->get_voice(event.command.guild_id);
//	voice->voiceclient->set_send_audio_type(dpp::discord_voice_client::satype_overlap_audio);
//	std::byte buf[11520];
//	// Bytes from output
//	size_t bytes_read;
//	// Get audio from song and pipe to discord
//	while ((bytes_read = fread(buf, sizeof(std::byte), 11520, pipe)) > 0) {
//		if (bytes_read < dpp::send_audio_raw_max_length) {
//			std::cout << "11520 was bigger than bytes read: " << bytes_read << std::endl;
//			continue;
//		}
//		// Send audio data if cringe is in an on-ready state
//		if (voice->voiceclient && voice->voiceclient->is_ready()) {
//			voice->voiceclient->send_audio_raw((uint16_t *) buf, sizeof(buf));
//		}
//	}
//	voice->voiceclient->insert_marker();
}
