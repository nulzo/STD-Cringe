#include "utils/http/cringe_ollama.h"

CringeOllama::CringeOllama(const std::string &endpoint) : endpoint(endpoint) {}

CringeOllama::~CringeOllama() = default;

std::string CringeOllama::chat(const std::string &model, const std::string &prompt) {
	std::string url = fmt::format("{}/api/v1/chat", endpoint);
	std::string sanitized = this->sanitize(prompt);
	json response = this->curl.post(fmt::format(R"({{ "chat": "{}", "model": "{}" }})", sanitized, model), url);
}

static std::string sanitize(const std::string &prompt) {
	size_t found = prompt.find('"');
	std::string sanitized = prompt;
	while (found != std::string::npos) {
		std::string replace = fmt::format(R"(\{})", "'");
		sanitized.replace(found, 1, replace);
		found = sanitized.find('\"', found + 2);
	}
	return sanitized;
}
