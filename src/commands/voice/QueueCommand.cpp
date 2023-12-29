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

#include "commands/voice/QueueCommand.h"
#include "utils/cringe.h"
#include <fmt/format.h>

dpp::slashcommand queue_declaration() {
	return dpp::slashcommand()
			.set_name("queue")
			.set_description("See the current music queue");
}

void queue_command(const dpp::slashcommand_t &event, Cringe::CringeQueue queue) {
	std::string embed_reason;
	dpp::embed embed;
	std::queue<Cringe::CringeSong> current_queue = queue.get_queue();

	// Loop through the temporary queue
	while (!current_queue.empty()) {
		// Access the front element
		Cringe::CringeSong frontElement = current_queue.front();
		embed_reason += fmt::format("**Title**: {}\n", frontElement.get_title());
		// Remove the front element from the temporary queue
		current_queue.pop();
	}
	embed.set_title("Current Queue").set_color(Cringe::CringeColor::CringeOrange).set_timestamp(time(nullptr)).add_field("Tracks", embed_reason).set_thumbnail(Cringe::CringeIcon::MusicIcon);
//	embed = status_embed("Current Queue", embed_reason, Cringe::CringeStatus::SUCCESS);
	dpp::message message(event.command.channel_id, embed);
	event.reply(message);
}