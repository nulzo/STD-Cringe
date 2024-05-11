#ifndef CRINGE_CRINGE_QUEUE_H
#define CRINGE_CRINGE_QUEUE_H

#include <queue>
#include "iostream"
#include "dpp/dpp.h"
#include "utils/audio/cringe_song.h"

class CringeQueue {
	int length;
public:
	[[nodiscard]] bool is_empty() const;
	void enqueue(CringeSong &song);
	CringeSong dequeue();
	std::string peek();
	std::queue<CringeSong> get_queue();
	void display_queue() const;
private:
	std::queue<CringeSong> cringe_queue;
};

#endif //CRINGE_CRINGE_QUEUE_H
