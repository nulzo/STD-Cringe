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

json cringe_history(const std::string &prompt, const std::string &model) {
	std::string endpoint = get_env("LOCAL_ENDPOINT") + "/api/v1/code";
	std::string sanitized_prompt = prompt;
	replace_raw_char(sanitized_prompt, '"');
	return cringe_post(fmt::format(R"({{ "chat": "{}", "model": "{}" }})",
								   sanitized_prompt, model), endpoint);
}

json cringe_describe(const std::string &url) {
	std::string endpoint = get_env("LOCAL_ENDPOINT") + "/api/v1/describe";
	return cringe_post(fmt::format(R"({{ "url": "{}"}})", url), endpoint);
}

json cringe_imagine(const std::string &prompt, const std::string &style) {
	std::string endpoint = get_env("LOCAL_ENDPOINT") + "/api/v1/generate";;
	return cringe_post(fmt::format(R"({{ "prompt": "{}", "style": "{}" }})", prompt, style), endpoint);
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
