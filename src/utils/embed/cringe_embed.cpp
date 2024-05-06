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

#include "utils/embed/cringe_embed.h"
#include "utils/misc/cringe.h"
#include "utils/misc/cringe_helpers.h"
#include "fmt/format.h"
#include <unordered_map>
#include "utils/audio/cringe_song.h"

dpp::embed info_embed(const std::string& title, const std::string& response, const std::string& avatar_url, const std::string& mention, const std::string& created, const std::string& joined_at, const std::string& premium, const std::string& nitro, const std::string& bot) {
	return dpp::embed()
			.set_color(Cringe::CringeColor::CringeDark)
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

dpp::embed confession_embed(const std::string& confession) {
	dpp::embed embed;
	embed.set_color(Cringe::CringeColor::CringePrimary);
	embed.set_title("Anonymous Confession");
	embed.set_thumbnail(Cringe::CringeIcon::ConfessionIcon);
	embed.set_description(confession);
	embed.set_timestamp(time(nullptr));
	return embed;
}

dpp::embed added_to_queue_embed(CringeSong song) {
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
	std::string comments = to_string(data["comments"]);
	std::string upvotes = to_string(data["upvotes"]);
	auto media = data["media"];
	auto e = data["media_embed"];
	float ratio = data["upvote_ratio"];
	dpp::embed embed;
	embed.set_color(Cringe::CringeColor::CringePrimary)
	.set_title("Cringe Crawler")
	.set_thumbnail(Cringe::CringeIcon::LightningIcon)
	.add_field("", "+-----------------------------------------------------------------+")
	.add_field("Subreddit", subreddit, true)
	.add_field("Author", author, true)
	.add_field("Posted", fmt::format("<t:{}:D>", creation), true)
	.add_field("Comments", comments, true)
	.add_field("Upvotes", upvotes, true)
	.add_field("Upvote Ratio", fmt::format("{}%", ratio * 100), true)
	.add_field("", "+-----------------------------------------------------------------+");

	if (!title.empty()) {
		embed.add_field("Title", title);
		embed.add_field("", "+-----------------------------------------------------------------+");
	}
	if (!description.empty()) {
		embed.add_field("Description", description);
		embed.add_field("", "+-----------------------------------------------------------------+");
	}
	embed.set_timestamp(time(nullptr));
	if(!media.empty() && media.contains("oembed")) {
		if (media["oembed"].contains("thumbnail_url")){
			embed.set_image(media["oembed"]["thumbnail_url"]);
		}
	}
	return embed;
}

CringeEmbed &CringeEmbed::setTitle(const std::string &embed_title) {
	embed.set_title(embed_title);
	return *this;
}

CringeEmbed::CringeEmbed() {
	embed = dpp::embed();
	embed.set_title(title)
	.set_color(color)
	.set_thumbnail(icon)
	.set_timestamp(time(nullptr))
	.set_footer(help, profile_pic);
}

CringeEmbed::~CringeEmbed() = default;

CringeEmbed &CringeEmbed::setHelp(const std::string &help_text) {
	embed.set_footer(help_text, profile_pic);
	return *this;
}

CringeEmbed &CringeEmbed::setIcon(const std::string &embed_icon) {
	embed.set_thumbnail(embed_icon);
	return *this;
}

CringeEmbed &CringeEmbed::setColor(const int &embed_color) {
	embed.set_color(embed_color);
	return *this;
}

CringeEmbed &CringeEmbed::setFields(const std::vector<std::vector<std::string>> &fields) {
	std::string part;
	int chunk_max = 1024;
	if(!fields.empty()) {
		for (const auto &field: fields) {
			if(field[1].length() >= chunk_max) {
				for (size_t i = 0; i < field[1].length(); i += chunk_max) {
					std::string chunk = field[1].substr(i, chunk_max);
					part = (i == 0) ? field[0] : "";
					embed.add_field(part, chunk);
				}
			} else {
				embed.add_field(field[0], field[1], field[2] == "true");
			}
		}
	}
	return *this;
}

CringeEmbed &CringeEmbed::setDescription(const std::string &embed_description) {
	embed.set_description(embed_description);
	return *this;
}

CringeEmbed &CringeEmbed::setImage(const std::string &embed_image) {
	embed.set_image(embed_image);
	return *this;
}

dpp::embed now_streaming(CringeSong song){
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
	channel = find_channel(event.from->get_voice(event.command.guild_id)->channel_id)->get_mention();
	footer = fmt::format("Requested by: {}", event.command.usr.global_name);
	// Set the title and assign it an icon
	embed.set_title("Now Streaming").set_thumbnail(Cringe::CringeIcon::MusicIcon);
	// Set the title field
	embed.add_field("Title", title);
	embed.add_field("Author", author);
	// Set the URL field
	embed.add_field("URL", song.get_url());
	// Set the first row of embed fields
	embed.add_field("Streaming in", channel, true).add_field("Duration", duration, true).add_field("Upload Date", fmt::format("<t:{}:D>", song.get_upload_date()), true);;
	// Set the image to the thumbnail of the YT video
	embed.set_image(song.get_thumbnail());
	embed.add_field("Comments", song.get_comments(), true).add_field("Views", song.get_view_count(), true).add_field("Subscribers", song.get_subscribers(), true);
	// Set the color of the embed
	embed.set_color(Cringe::CringeColor::CringePrimary);
	// Set the footer to tell the server who requested the song
	embed.set_footer(footer, event.command.usr.get_avatar_url());
	// Add a timestamp to the embed
	embed.set_timestamp(time(nullptr));

	return embed;
}

CringeEmbed cringe_success_embed(std::string &reason) {
	CringeEmbed embed;
	embed.setTitle("Success");
	embed.setColor(Cringe::CringeColor::CringeSuccess);
	embed.setIcon(Cringe::CringeIcon::SuccessIcon);
	embed.setDescription(reason);
	return embed;
}

CringeEmbed cringe_warning_embed(std::string &reason) {
	CringeEmbed embed;
	embed.setTitle("Warning");
	embed.setColor(Cringe::CringeColor::CringeWarning);
	embed.setIcon(Cringe::CringeIcon::WarningIcon);
	embed.setDescription(reason);
	return embed;
}

CringeEmbed cringe_error_embed(std::string &reason) {
	CringeEmbed embed;
	embed.setTitle("Error");
	embed.setColor(Cringe::CringeColor::CringeError);
	embed.setIcon(Cringe::CringeIcon::ErrorIcon);
	embed.setDescription(reason);
	return embed;
}
