/*
 * MIT License
 *
 * Copyright (c) 2023 @nulzo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "utils/cringe.h"

void Cringe::CringeQueue::enqueue(const CringeSong& song) {
	cringe_queue.push(song);
	length++;
}

Cringe::CringeSong Cringe::CringeQueue::dequeue() {
	length--;
	Cringe::CringeSong song = cringe_queue.front();
	cringe_queue.pop();
	return song;
}

std::queue<Cringe::CringeSong> Cringe::CringeQueue::get_queue() {
	return cringe_queue;
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
