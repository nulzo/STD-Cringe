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

#include "utils/util.h"
#include "utils/embed.h"

dpp::slashcommand reddit_declaration() {
	return dpp::slashcommand()
			.set_name("reddit")
			.set_description("Get the top post from a subreddit")
			.add_option(dpp::command_option(dpp::co_string, "subreddit", "Subreddit to get post from", true))
			.add_option(dpp::command_option(dpp::co_string, "top", "Get the top post of timeframe", true)
								.add_choice(dpp::command_option_choice("Day", std::string("day")))
								.add_choice(dpp::command_option_choice("Month", std::string("month")))
								.add_choice(dpp::command_option_choice("Year", std::string("year")))
								.add_choice(dpp::command_option_choice("All-Time", std::string("all"))));

}

json parse_reddit_response(const std::string &res) {
	json pre_response;
	json response;
	pre_response = json::parse(res);
	pre_response = pre_response["data"]["children"][0]["data"];
	response["subreddit"] = pre_response["subreddit_name_prefixed"];
	response["title"] = pre_response["title"];
	response["author"] = pre_response["author"];
	response["description"] = pre_response["selftext"];
	response["upvote_ratio"] = pre_response["upvote_ratio"];
	response["time_created"] = pre_response["created"];
	response["url"] = pre_response["url"];
	response["media"] = pre_response["media"];
	response["embed"] = pre_response["media_embed"];
	response["media_embed"] = pre_response["secure_media_embed"];
	response["upvotes"] = pre_response["ups"];
	response["comments"] = pre_response["num_comments"];
	return response;
}

void reddit_command(dpp::cluster &bot, const dpp::slashcommand_t &event) {
	event.thinking();
	std::string subreddit = std::get<std::string>(event.get_parameter("subreddit"));
	std::string top = std::get<std::string>(event.get_parameter("top"));
	std::string response = get_reddit_response(subreddit, top);
	json response_data = parse_reddit_response(response);
	dpp::message msg(event.command.channel_id, reddit_embed(response_data).set_footer(fmt::format("Queried by: {}", event.command.usr.username), event.command.usr.get_avatar_url()));
	event.edit_original_response(msg);
}