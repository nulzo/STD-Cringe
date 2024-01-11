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

#include "utils/embed.h"
#include "utils/cringe.h"
#include "utils/util.h"
#include "fmt/format.h"

dpp::embed status_embed(const std::string &title, const std::string &reason, int status) {
	dpp::embed embed = dpp::embed();
	switch (status) {
		case Cringe::CringeStatus::ERROR:
			embed.set_color(Cringe::CringeColor::CringeError).set_thumbnail(Cringe::CringeIcon::ErrorIcon).set_description("An error has occurred due to the following exceptions");
			break;
		case Cringe::CringeStatus::WARNING:
			embed.set_color(Cringe::CringeColor::CringeWarning).set_thumbnail(Cringe::CringeIcon::WarningIcon).set_description("A warning has occurred due to the following exceptions");
			break;
		case Cringe::CringeStatus::SUCCESS:
			embed.set_color(Cringe::CringeColor::CringeSuccess).set_thumbnail(Cringe::CringeIcon::SuccessIcon).set_description("A success has been issued due to the following");
			break;
	}
	return embed.set_title(title).add_field("Reason", reason).set_timestamp(time(nullptr));
}

dpp::embed playing_embed(int status) {
	dpp::embed embed;
	embed.set_color(Cringe::CringeColor::CringeOrange).set_thumbnail(Cringe::CringeIcon::MusicIcon).set_timestamp(time(nullptr));
	embed.set_title("Now Streaming").set_description("Now streaming to your channel!").set_timestamp(time(nullptr));
	return embed;
}

dpp::embed info_embed(const std::string& title, const std::string& response, const std::string& avatar_url, const std::string& mention, const std::string& created, const std::string& joined_at, const std::string& premium, const std::string& nitro, const std::string& bot) {
	return dpp::embed()
			.set_color(Cringe::CringeColor::CringeBlack)
			.set_title(title)
			.set_description(response)
			.set_thumbnail(avatar_url)
			.add_field("Username", mention, true)
			.add_field("Account Created", created, true)
			.add_field("Joined Server", joined_at, true)
			.add_field("Booster", premium, true)
			.add_field("Nitro", nitro, true)
			.add_field("Bot", bot, true)
			.set_timestamp(time(nullptr));
}

dpp::embed generic_embed(const std::string &icon, const std::string &title, const std::string &description, int color){
	dpp::embed embed;
	embed.set_color(color);
	embed.set_description(description);
	embed.set_thumbnail(icon);
	embed.set_title(title);
	embed.set_timestamp(time(nullptr));
	return embed;
}

dpp::embed confession_embed(const std::string& confession) {
	dpp::embed embed;
	embed.set_color(Cringe::CringeColor::CringeIndigo);
	embed.set_title("Anonymous Confession");
	embed.set_thumbnail(Cringe::CringeIcon::ConfessionIcon);
	embed.set_description(confession);
	embed.set_timestamp(time(nullptr));
	return embed;
}

dpp::embed added_to_queue_embed(Cringe::CringeSong song) {
	dpp::embed embed;
	embed.set_title("Successfully added").set_color(Cringe::CringeColor::CringeSuccess).set_thumbnail(Cringe::CringeIcon::SuccessIcon);
	embed.add_field("Title", song.get_title()).add_field("URL", song.get_url());
	return embed;
}

dpp::embed reddit_embed(json data) {
	std::string author = data["author"];
	std::string subreddit = data["subreddit"];
	std::string title = data["title"];
	std::string description = data["description"];
	int creation = data["time_created"];
	std::string downvotes = to_string(data["downvotes"]);
	std::string upvotes = to_string(data["upvotes"]);
	float ratio = data["upvote_ratio"];
	dpp::embed embed;
	embed.set_color(Cringe::CringeColor::CringeOrange)
	.set_title("Cringe Crawler")
	.set_thumbnail(Cringe::CringeIcon::SnailIcon)
	.add_field("Subreddit", subreddit, true)
	.add_field("Author", author, true)
	.add_field("Posted", fmt::format("<t:{}:D>", creation), true)
	.add_field("Upvotes", upvotes, true)
	.add_field("Downvotes", downvotes, true)
	.add_field("Upvote Ratio", fmt::format("{}%", ratio * 100), true);
	if (!title.empty()) {
		embed.add_field("Title", title);
	}
	if (!description.empty()) {
		embed.add_field("Description", description);
	}
	embed.set_timestamp(time(nullptr));
	return embed;
}

dpp::embed now_streaming(Cringe::CringeSong song){
	// Define variables
	dpp::embed embed;
	std::string title;
	std::string duration;
	std::string author;
	std::string footer;
	std::string channel;
	dpp::slashcommand_t event;

	// Get data from the song object
	title = song.get_title();
	duration = seconds_to_formatted_time(atoi(song.get_formatted_duration().c_str()));
	author = song.get_artist();
	event = song.get_event();
	channel = event.command.channel.get_mention();
	footer = fmt::format("Requested by: {}", event.command.usr.global_name);

	// Set the title and assign it an icon
	embed.set_title("Now Streaming").set_thumbnail(Cringe::CringeIcon::MusicIcon);
	// Set the title field
	embed.add_field("Title", title);
	// Set the URL field
	embed.add_field("URL", song.get_url());
	// Set the first row of embed fields
	embed.add_field("Channel", channel, true).add_field("Duration", duration, true).add_field("Author", author, true);
	// Set the image to the thumbnail of the YT video
	embed.set_image(song.get_thumbnail());
	// Set the color of the embed
	embed.set_color(Cringe::CringeColor::CringeOrange);
	// Set the footer to tell the server who requested the song
	embed.set_footer(footer, event.command.usr.get_avatar_url());
	// Add a timestamp to the embed
	embed.set_timestamp(time(nullptr));

	return embed;
}