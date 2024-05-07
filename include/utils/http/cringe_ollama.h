#ifndef CRINGE_CRINGE_OLLAMA_H
#define CRINGE_CRINGE_OLLAMA_H

#include <dpp/dpp.h>
#include <iostream>
#include <string>
#include <fmt/core.h>
#include <regex>
#include <curl/curl.h>
#include "cringe_curl.h"

class CringeOllama {
public:
	CringeOllama(const std::string &endpoint);
	~CringeOllama();
	json chat(const std::string &model, const std::string &prompt);
private:
	CringeCurl curl;
	std::string endpoint;
	static std::string sanitize(const std::string &prompt);
};

#endif
