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

#include "commands/voice/queue_command.h"

dpp::slashcommand queue_declaration() {
    return dpp::slashcommand().set_name("queue").set_description(
        "See the current music queue");
}

void queue_command(const dpp::slashcommand_t &event, CringeQueue queue) {
    std::string embed_reason;
    dpp::embed embed;
    std::queue<CringeQueueContents> current_queue = queue.get_queue();
    int total_minutes = 0;
    int total_songs = 0;
//    // Loop through the temporary queue
//    while (!current_queue.empty()) {
//        // Access the front element
//        CringeSong song = current_queue.front();
//        total_minutes += atoi(song.duration.c_str());
//        std::string song_duration = song.duration.c_str();
//        embed_reason += fmt::format("\n**Title**: {}\n**Length**: {}\n",
//                                    song.title, song_duration);
//        total_songs++;
//        // Remove the front element from the temporary queue
//        current_queue.pop();
//    }
//    embed.add_field(
//        "Queue Info",
//        fmt::format("\n**Total Songs**: {}\n**Queue Duration**: {}\n",
//                    total_songs, total_minutes));
    embed.set_title("Current Queue")
        .set_color(CringeColor::CringePrimary)
        .add_field("Tracks", embed_reason)
        .set_timestamp(time(nullptr))
        .set_thumbnail(CringeIcon::MusicIcon);
    dpp::message message(1081850403920035931, embed);
    event.reply(message);
}
