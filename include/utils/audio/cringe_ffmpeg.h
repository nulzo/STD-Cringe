#ifndef CRINGE_CRINGE_FFMPEG_H
#define CRINGE_CRINGE_FFMPEG_H

#include <fmt/format.h>
#include <iostream>
#include <string>
#include <map>

const std::map<std::string, std::string> filters = {
		{"Vaporwave", "highpass=f=50,lowpass=f=2750,aresample=48000,asetrate=48000*0.85,bass=g=5:f=110:w=0.6"},
		{"4am in the Bathroom", "highpass=f=10,lowpass=f=400,aresample=44100,asetrate=44100*0.85,bass=g=4:f=110:w=0.6,alimiter=1"},
		{"Nightcore", "aresample=48000,asetrate=48000*1.5"},
		{"Bassboost", "bass=g=9:f=160:w=0.8"},
		{"What the...", "aresample=48000,asetrate=48000,acrusher=level_in=2:level_out=4,bass=g=17:f=120:w=0.5"},
		{"Lofi Hip Hop Beats to Relax/Study to", "aresample=48000,asetrate=48000*0.9,extrastereo=m=2.5:c=disabled"},
		{"Widen Audio", "compand=attacks=0:points=-80/-169|-54/-80|-49.5/-64.6|-41.1/-41.1|-25.8/-15|-10.8/-4.5|0/0|20/8.3"},
		{"Sigma Type Beat", "aresample=48000,asetrate=48000*0.8,bass=g=8:f=110:w=0.6"},
		{"Paper Cut", "aexciter=level_in=3:level_out=3:amount=3:drive=8:blend=8,bass=g=2:f=100:w=0.6"}
};

class CringeFFMPEG {
public:
	CringeFFMPEG();
	~CringeFFMPEG();
	auto set_filter(const std::string &filter) -> std::string;
	auto get_stream(const std::string &filter = "") -> std::string;
};

#endif
