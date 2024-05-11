#ifndef CRINGE_CRINGE_QUEUE_H
#define CRINGE_CRINGE_QUEUE_H

#include <queue>
#include "iostream"
#include "dpp/dpp.h"
#include "utils/audio/cringe_song.h"

struct CringeQueueContents {
	std::string request;
	std::string filter;
};

class CringeQueue {
	int length;
public:
	[[nodiscard]] bool is_empty() const;
	void enqueue(const std::string &request, const std::string &filter);
	CringeQueueContents dequeue();
	std::string peek();
	std::queue<CringeQueueContents> get_queue();
	void display_queue() const;
private:
	std::queue<CringeQueueContents> cringe_queue;
};

#endif //CRINGE_CRINGE_QUEUE_H
