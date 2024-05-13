#include "utils/audio/cringe_ffmpeg.h"

CringeFFMPEG::CringeFFMPEG() = default;
CringeFFMPEG::~CringeFFMPEG() = default;

auto CringeFFMPEG::set_filter(const std::string &filter) -> std::string {
	auto cringe_filter = filters.find(filter);
	std::cout << filter << "\n";
	return (cringe_filter != filters.end()) ? fmt::format("-filter_complex {}", cringe_filter->second) : "";
}

auto CringeFFMPEG::get_stream(const std::string &filter) -> std::string {
	if(!filter.empty()){
		return fmt::format("ffmpeg -i pipe:0 -hide_banner -loglevel warning {} -f s16le -ac 2 -ar 48000 pipe:1", this->set_filter(filter));
	}
	return "ffmpeg -i pipe:0 -hide_banner -loglevel warning -f s16le -ac 2 -ar 48000 pipe:1";
}
