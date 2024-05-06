#include "utils/audio/cringe_audio_streaming.h"

#include <fmt/format.h>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
#include <opus/opus.h>
}

void cringe_streamer(const std::string &ffmpeg_data, dpp::voiceconn *voice) {
    // We need to set this otherwise the bot attempts to send all packets at
    // once
    voice->voiceclient->set_send_audio_type(
        dpp::discord_voice_client::satype_overlap_audio);
    // buf to store contents
    std::byte buf[11520];
    // Subprocess fork that runs our ffmpeg piped audio
    auto pipe = popen(ffmpeg_data.c_str(), "r");
    // Bytes from output
    size_t bytes_read;
    // Get audio from song and pipe to discord
    while ((bytes_read = fread(buf, sizeof(std::byte),
                               dpp::send_audio_raw_max_length, pipe)) > 0) {
        if (bytes_read >= dpp::send_audio_raw_max_length) {
            // Send audio data if cringe is in an on-ready state
            if (voice->voiceclient && voice->voiceclient->is_ready()) {
                voice->voiceclient->send_audio_raw((uint16_t *)buf,
                                                   sizeof(buf));
            }
        }
    }
    voice->voiceclient->insert_marker();
    pclose(pipe);
}
