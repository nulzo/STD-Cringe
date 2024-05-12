#include "utils/http/cringe_ollama.h"

struct OllamaChat {
	std::string model;
	std::string prompt;
};

CringeOllama::CringeOllama(const std::string &endpoint) : endpoint(endpoint) {}

CringeOllama::~CringeOllama() = default;

json CringeOllama::chat(const std::string &model, const std::string &prompt) {
	std::string url = fmt::format("{}/api/generate", endpoint);
	std::string sanitized = this->sanitize(prompt);
	std::string query = fmt::format(R"({{ "model": "{}", "prompt": "{}", "stream": false }})", model, sanitized);
	json response = this->curl.post(query, url);
	return response;
}

json CringeOllama::list() {
	std::string url = fmt::format("{}/api/tags", endpoint);
	json response = this->curl.get(url);
	return response;
}

std::string CringeOllama::sanitize(const std::string &prompt) {
	size_t found = prompt.find('"');
	std::string sanitized = prompt;
	while (found != std::string::npos) {
		std::string replace = fmt::format(R"(\{})", '"');
		sanitized.replace(found, 1, replace);
		found = sanitized.find('\"', found + 2);
	}
	return sanitized;
}
