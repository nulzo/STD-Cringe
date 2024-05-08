#include "utils/audio/cringe_audio.h"
#include "utils/audio/cringe_ffmpeg.h"
#include "utils/audio/cringe_youtube.h"

CringeAudioStreamer::CringeAudioStreamer() {}

void CringeAudioStreamer::stream(dpp::voiceconn *voice, const std::string &request, const std::string &filter) {
	CringeYoutube cringe_youtube;
	CringeFFMPEG cringe_ffmpeg;

	std::string source = cringe_youtube.search(request);
	std::string ffmpeg = cringe_ffmpeg.get_stream(filter);
	std::string process = fmt::format("{} | {}", source, ffmpeg);

	voice->voiceclient->set_send_audio_type(dpp::discord_voice_client::satype_overlap_audio);
	std::byte buf[11520];
	auto pipe = popen(process.c_str(), "r");
	size_t bytes_read;
	while ((bytes_read = fread(buf, sizeof(std::byte), dpp::send_audio_raw_max_length, pipe)) > 0) {
		if (bytes_read >= dpp::send_audio_raw_max_length) {
			if (voice->voiceclient && voice->voiceclient->is_ready()) {
				voice->voiceclient->send_audio_raw((uint16_t *)buf, sizeof(buf));
			}
		}
	}
	voice->voiceclient->insert_marker();
	pclose(pipe);
}

CringeAudioStreamer::~CringeAudioStreamer() = default;
