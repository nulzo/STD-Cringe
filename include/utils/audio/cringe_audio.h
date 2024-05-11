#ifndef CRINGE_CRINGE_AUDIO_H
#define CRINGE_CRINGE_AUDIO_H

#include <iostream>
#include <string>
#include <dpp/dpp.h>
#include "utils/audio/cringe_song.h"

class CringeAudioStreamer {
public:
	CringeAudioStreamer();
	~CringeAudioStreamer();
	void stream(dpp::voiceconn *voice, CringeSong &song);
};

#endif
