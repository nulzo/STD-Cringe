#include "utils/cringe.h"

void Cringe::CringeQueue::enqueue(const CringeSong& song) {
	cringe_queue.push(song);
	length++;
}

void Cringe::CringeQueue::dequeue() {
	if (!is_empty()) {
		cringe_queue.pop();
		length--;
	}
}

void Cringe::CringeQueue::display_queue() const {
	if (!is_empty()) {
		std::cout << "Song Queue:" << std::endl;
		std::queue<Cringe::CringeSong> t_q = cringe_queue;
	} else {
		std::cout << "Queue is empty." << std::endl;
	}
}

bool Cringe::CringeQueue::is_empty() const {
	return cringe_queue.empty();
}

std::string Cringe::CringeQueue::peek() {
	if (!is_empty()) {
		return cringe_queue.front().get_title();
	} else return "";
}
