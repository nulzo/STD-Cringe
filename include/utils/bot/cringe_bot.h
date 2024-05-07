#ifndef CRINGE_CRINGEBOT_H
#define CRINGE_CRINGEBOT_H

#include "utils/cringe_logger.h"
#include "utils/audio/cringe_queue.h"
#include "utils/cringe_logger.h"
#include "utils/http/cringe_ollama.h"

class CringeBot {
private:
	std::string _token;
public:
	CringeQueue queue;
	CringeOllama ollama;
	dpp::cluster cluster;
	std::shared_ptr<spdlog::logger> cringe_logger;
	explicit CringeBot(const std::string &token, CringeOllama ollama);
	~CringeBot();
};

#endif
