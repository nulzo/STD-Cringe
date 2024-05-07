#ifndef CRINGE_CRINGE_FFMPEG_H
#define CRINGE_CRINGE_FFMPEG_H

#include <string>
#include <iostream>
#include <map>

class CringeFFMPEG {
public:
	CringeFFMPEG();
	~CringeFFMPEG();
	std::string set_filter(const std::string &filter);
	std::string get_stream();
};


#endif
