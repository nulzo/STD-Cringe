#ifndef CRINGE_CRINGE_AUDIO_STREAMER_H
#define CRINGE_CRINGE_AUDIO_STREAMER_H

#include <string>
#include <dpp/dpp.h>

void cringe_streamer(const std::string &ffmpeg_data, dpp::voiceconn *voice);

#endif //CRINGE_CRINGE_AUDIO_STREAMER_H
