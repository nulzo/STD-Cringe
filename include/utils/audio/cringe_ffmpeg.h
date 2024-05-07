#ifndef CRINGE_CRINGE_FFMPEG_H
#define CRINGE_CRINGE_FFMPEG_H

#include <string>
#include <iostream>
#include <map>

class CringeFFMPEG {
public:
	CringeFFMPEG();
	~CringeFFMPEG();
	auto set_filter(const std::string &filter) -> std::string;
	auto get_stream(const std::string &filter = "") -> std::string;
};

#endif
