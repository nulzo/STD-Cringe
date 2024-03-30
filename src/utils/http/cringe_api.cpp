#include "utils/misc/cringe.h"
#include "utils/misc/cringe_helpers.h"
#include "utils/http/cringe_request.h"

json cringe_chat(const std::string &prompt, const std::string &model) {
	std::string endpoint = get_env("LOCAL_ENDPOINT") + "/api/v1/chat";
	std::string sanitized_prompt = prompt;
	replace_raw_char(sanitized_prompt, '"');
	return cringe_post(fmt::format(R"({{ "chat": "{}", "model": "{}" }})",
								 sanitized_prompt, model), endpoint);
}

std::string cringe_describe(const std::string &url) {
	std::string endpoint = get_env("LOCAL_ENDPOINT");
	endpoint = endpoint + "/api/v1/describe";
	auto res = cringe_post(fmt::format(R"({{ "url": "{}"}})", url), endpoint);
	return res.contains("response") ? res["response"] : res["error"];
}

std::string get_image(const std::string &prompt) {
	std::string endpoint = get_env("LOCAL_ENDPOINT");
	endpoint = endpoint + "/api/v1/generate";
	auto res = cringe_post(fmt::format(R"({{ "prompt": "{}"}})", prompt), endpoint);
	return res["response"];
}


std::string get_reddit_response(const std::string &subreddit, const std::string &filter) {
	std::string URL = fmt::format("https://old.reddit.com/r/{}/top/.json?sort=top&limit=1&t={}", subreddit, filter);
	// Get the command to extract the URL
	std::string command = fmt::format("curl -X GET -L \"{}\"", URL);
	char buffer[128];
	std::string response;
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

	return response;
}
