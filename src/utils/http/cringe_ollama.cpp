#include "utils/http/cringe_ollama.h"

CringeOllama::CringeOllama(const std::string &endpoint) : endpoint(endpoint) {}

CringeOllama::~CringeOllama() = default;

json CringeOllama::chat(const std::string &model, const std::string &prompt) {
	std::string url = fmt::format("{}/api/v1/chat", endpoint);
	std::string sanitized = this->sanitize(prompt);
	std::string query = fmt::format(R"({{ "chat": "{}", "model": "{}" }})", model, sanitized);
	json response = this->curl.post(query, url);
	return response;
}

json CringeOllama::list() {
	std::string url = fmt::format("{}/api/v1/list", endpoint);
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
