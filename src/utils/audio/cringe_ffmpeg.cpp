#include <fmt/format.h>
#include "utils/audio/cringe_ffmpeg.h"

const std::map<std::string, std::string> filters = {
	{"vaporwave", "\"highpass=f=50, lowpass=f=2750, aresample=48000, asetrate=48000*0.85,bass=g=5:f=110:w=0.6, compand=attacks=0:points=-80/-169|-54/-80|-49.5/-64.6|-41.1/-41.1|-25.8/-15|-10.8/-4.5|0/0|20/8.3\""},
	{"bathroom", "\"highpass=f=10, lowpass=f=400, aresample=44100, asetrate=44100*0.85,bass=g=4:f=110:w=0.6, alimiter=1, compand=attacks=0:points=-80/-169|-54/-80|-49.5/-64.6|-41.1/-41.1|-25.8/-15|-10.8/-4.5|0/0|20/8.3\""},
	{"nightcore", "\"aresample=48000, asetrate=48000*1.5\""},
	{"bassboot", "\"bass=g=20:f=110:w=0.3\""},
	{"dim", "apulsator=hz=0.09"},
	{"lofi", "aresample=48000,asetrate=48000*0.9,extrastereo=m=2.5:c=disabled"},
	{"nosilence", "\"silenceremove=1:0:-50dB\""},
	{"expand", "\"compand=attacks=0:points=-80/-169|-54/-80|-49.5/-64.6|-41.1/-41.1|-25.8/-15|-10.8/-4.5|0/0|20/8.3\""}
};

CringeFFMPEG::CringeFFMPEG() {}
CringeFFMPEG::~CringeFFMPEG() = default;

std::string CringeFFMPEG::set_filter(const std::string &filter) {
	auto cringe = filters.find(filter);
	return fmt::format("-filter_complex {}", filter);
}

std::string CringeFFMPEG::get_stream() {

}
