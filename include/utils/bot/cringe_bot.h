#ifndef CRINGE_CRINGEBOT_H
#define CRINGE_CRINGEBOT_H

#include "utils/audio/cringe_queue.h"
#include "utils/listeners/cringe_logger.h"
#include "utils/http/cringe_ollama.h"

class CringeBot {
private:
	std::string _token;
public:
	std::vector<std::string> models = {"cringe", "ethan", "klim", "joeman"};
	CringeQueue queue;
	CringeOllama ollama;
	dpp::cluster cluster;
	CringeLogger cringe_logger = CringeLogger();
	explicit CringeBot(const std::string &token, CringeOllama ollama);
	~CringeBot();
};

#endif
