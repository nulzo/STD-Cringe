#ifndef CRINGE_CRINGEBOT_H
#define CRINGE_CRINGEBOT_H

#include "utils/cringe_logger.h"
#include "utils/audio/cringe_queue.h"
#include "utils/cringe_logger.h"
#include "utils/misc/cringe.h"
#include "utils/misc/cringe_helpers.h"

class CringeBot {
private:
	std::string _token;
public:
	CringeQueue queue;
	dpp::cluster cluster;
	std::shared_ptr<spdlog::logger> cringe_logger;
	explicit CringeBot(const std::string &token);
	~CringeBot();
};

#endif
