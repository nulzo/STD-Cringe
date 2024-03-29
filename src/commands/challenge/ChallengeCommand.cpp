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

#include "commands/challenge/challenge.h"
#include "utils/cringe.h"

using json = nlohmann::json;

dpp::slashcommand problem_declaration() {
	return dpp::slashcommand()
			.set_name("problem")
			.set_description("Answer a programming challenging")
			.add_option(dpp::command_option(dpp::co_string, "difficulty", "The difficulty of the question", true)
								.add_choice(dpp::command_option_choice("Easy", std::string("easy")))
								.add_choice(dpp::command_option_choice("Medium", std::string("medium")))
								.add_choice(dpp::command_option_choice("Hard", std::string("hard"))
								)
			);

}

void problem_command(dpp::cluster &bot, const dpp::slashcommand_t &event) {
	const char *filename = "problems.json";

	std::ifstream file(filename);

	json data = json::parse(file);

	std::vector<json> questions = {};

	std::string difficulty = std::get<std::string>(event.get_parameter("difficulty"));

	int color = Cringe::CringeColor::CringeDark;

	int upper_threshold = 0;

	int lower_threshold = 0;

	if (difficulty == "easy") {
		color = Cringe::CringeColor::CringeDark;
		upper_threshold = 5;
	}

	if (difficulty == "medium") {
		color = Cringe::CringeColor::CringeDark;
		upper_threshold = 45;
		lower_threshold = 5;
	}

	if (difficulty == "hard") {
		color = Cringe::CringeColor::CringeDark;
		upper_threshold = 1000;
		lower_threshold = 45;
	}

	for (auto &i: data) {
		if (i["difficulty"] <= upper_threshold && i["difficulty"] > lower_threshold) {
			questions.push_back(i);
		}
	}

	std::random_device r;

	std::default_random_engine e1(r());

	std::uniform_int_distribution uniform_dist(0, (int) questions.size() - 1);

	int random = uniform_dist(e1);

	const json problem = questions.at(random);

	std::string description = problem["description"];
	std::string name = problem["name"];
	std::string id = std::to_string(static_cast<int>(problem["id"]));
	std::string points = std::to_string(static_cast<int>(problem["difficulty"]));
	std::regex regexPattern("\n\n");

	auto regexBegin = std::sregex_iterator(description.begin(), description.end(), regexPattern);
	auto regexEnd = std::sregex_iterator();
	int last_index = 0;
	for (auto it = regexBegin; it != regexEnd; ++it) {
		const std::smatch &match = *it;
		last_index = (int) match.position();
	}
	const std::string question = std::string(description).substr(0, last_index);
	const std::string prompt = std::string(description).substr(last_index, description.size());

	dpp::embed embed = dpp::embed()
			.set_color(color)
			.set_title("Problem " + id + ": " + name)
			.add_field("Difficulty", difficulty, true)
			.add_field("Points", points, true)
			.add_field("Description", question)
			.add_field("Question", prompt);

	dpp::message msg(event.command.channel_id, embed);

	msg.add_component(
			dpp::component().add_component(
					dpp::component()
							.set_label("Solve ")
							.set_type(dpp::cot_button)
							.set_style(dpp::cos_primary)
							.set_id("button_id")
			)
	);

	bot.on_button_click([](const dpp::button_click_t &event) {
		dpp::interaction_modal_response modal("solution_modal", "Your Solution");
		modal.add_component(dpp::component().set_label("Solution").set_id("solution_id").set_type(dpp::cot_text).set_placeholder("Your solution...").set_min_length(1).set_max_length(100).set_text_style(dpp::text_short));
		modal.add_row();
		modal.add_component(dpp::component().set_label("Your Code (optional)").set_id("code_id").set_type(dpp::cot_text).set_placeholder("Your code...").set_min_length(0).set_max_length(2000).set_text_style(dpp::text_paragraph));
		event.dialog(modal);
	});

	bot.on_form_submit([](const dpp::form_submit_t &event) {
		std::string v = std::get<std::string>(event.components[0].components[0].value);
		dpp::embed embed = dpp::embed().set_color(Cringe::CringeColor::CringePrimary).set_title(":x: Wrong Answer :x:").set_description(event.command.get_issuing_user().global_name);
		dpp::message m(event.command.channel_id, embed);
		/* Emit a reply. Form submission is still an interaction and must generate some form of reply! */
		event.reply(m);
	});

	/* Reply to the user with the message, containing our embed. */
	event.reply(msg);
}