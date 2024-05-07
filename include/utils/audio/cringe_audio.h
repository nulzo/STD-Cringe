#ifndef CRINGE_CRINGE_AUDIO_H
#define CRINGE_CRINGE_AUDIO_H

#include <iostream>
#include <string>
#include <dpp/dpp.h>

class CringeAudioStreamer {
public:
	CringeAudioStreamer();
	~CringeAudioStreamer();
	void stream(dpp::voiceconn *voice, const std::string &request, const std::string &filter);
};

#endif
