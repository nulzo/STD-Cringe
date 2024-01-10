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

dpp::slashcommand image_declaration() {
	return dpp::slashcommand()
			.set_name("image")
			.set_description("Have cringe generate an image")
			.add_option(dpp::command_option(dpp::co_string, "prompt", "Prompt for image", true));
}

std::string image_request(const std::string &prompt) {
	const char *request;
	int return_value;
	std::string response;
	std::string url = "https://api.openai.com/v1/images/generations";
	std::string auth;
	std::string json_payload;
	std::string system_role;
	json responseJson;
	get_env("OPEN_AI_AUTH", auth);
	auth = fmt::format("Authorization: Bearer {}", auth);
	const char *URL = url.c_str();
	const char *AUTH = auth.c_str();
	json_payload = fmt::format(R"({{"model": "dall-e-3", "prompt": "{}", "n": 1, "size": "1024x1024"}})", prompt);
	request = json_payload.c_str();
//	return_value = get_request(request, URL, AUTH, response);
	responseJson = json::parse(response);
	return responseJson["data"][0]["url"];
}

void image_command(dpp::cluster &bot, const dpp::slashcommand_t &event) {
	std::string prompt = std::get<std::string>(event.get_parameter("prompt"));
	event.thinking(true);
	dpp::message msg(event.command.channel_id, "We are sorry, but this feature is currently disabled due to it sucking ass.");
	event.edit_original_response(msg);
//	std::string response = image_request(prompt);
//	dpp::embed embed;
//	embed.set_title("Generated Image").set_image(response).add_field("Prompt", prompt).set_color(Cringe::CringeColor::CringeSuccess);
//	dpp::message msg(event.command.channel_id, embed);
//	event.edit_original_response(msg);
}