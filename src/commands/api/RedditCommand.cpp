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

#include "commands/api/api.h"
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

void reddit_command(dpp::cluster &bot, const dpp::slashcommand_t &event) {
	std::string subreddit = std::get<std::string>(event.get_parameter("subreddit"));
	std::string top = std::get<std::string>(event.get_parameter("top"));
	std::string URL = fmt::format("https://www.reddit.com/r/{}/top/.json?sort=top&limit=1&t={}", subreddit, top);
	event.thinking();
	json responseJson;
	// Allocate c style buf to store result of command
	char buffer[128];
	// Var to store processed URL
	std::string response;
	// Get the command to extract the URL
	std::string command = fmt::format("curl -X GET -L \"{}\"", URL);
	// Convert to a C-string
	const char *cmd = command.c_str();
	// Open the pipe to process to command
	FILE *pipe = popen(cmd, "r");
	// Check that the pipe was opened successfully
	if (!pipe) {
		std::cerr << "Error opening pipe" << std::endl;
	}
	// Write contents of stdout buf to c++ style string
	while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
		response += buffer;
	}

	responseJson = json::parse(response);
	std::cout << responseJson;
//
//	std::cout << responseJson["data"]["children"][0] << "\n";
//	std::cout << responseJson["data"]["children"][0]["data"]["title"] << "\n";

	std::string title = responseJson["data"]["children"][0]["data"]["title"];
	std::string author = responseJson["data"]["children"][0]["data"]["author"];
	std::string description = responseJson["data"]["children"][0]["data"]["selftext"];
	std::string thumbnail = responseJson["data"]["children"][0]["data"]["thumbnail"];
	std::string header = fmt::format("r/{} :: {}", subreddit, author);
	std::string image = responseJson["data"]["children"][0]["data"]["url_overridden_by_dest"];

	dpp::embed e;
	e.set_thumbnail(thumbnail)
	.set_color(Cringe::CringeColor::CringeOrange)
	.set_title("r/" + subreddit)
	.add_field("Title", title, true)
	.add_field("Description", description).set_image(image);

	dpp::message msg(event.command.channel_id, e);

	event.edit_original_response(msg);
}