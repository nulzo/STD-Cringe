#include "commands/voice.h"
#include "utils/embed.h"
#include "utils/cringe.h"
#include <algorithm>
#include <regex>
#include <fmt/format.h>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswresample/swresample.h>
#include <opus/opus.h>
}


dpp::slashcommand join_declaration() {
	return dpp::slashcommand()
			.set_name("join")
			.set_description("Join the voice channel you are in")
			.add_option(dpp::command_option(dpp::co_channel, "channel", "The VC for bot to join (defaults to your current channel)", false));
}

dpp::slashcommand play_declaration() {
	return dpp::slashcommand()
			.set_name("play")
			.set_description("Play a song in the voice channel you are in")
			.add_option(dpp::command_option(dpp::co_string, "song", "Song you wish to stream (either query or URL)", true));
}

void join_command(dpp::cluster &bot, const dpp::slashcommand_t &event) {
	std::string embed_reason;
	dpp::embed embed;
	dpp::guild *guild;
	dpp::snowflake parameter;
	dpp::channel requested_channel;

	// Get the guild of issued command
	guild = dpp::find_guild(event.command.guild_id);

	// Set flag if the channel has been found
	auto p = event.get_parameter("channel");
	if (p.index() != 0) {
		parameter = std::get<dpp::snowflake>(event.get_parameter("channel"));
		// Get the param (if any) and find voice channel id
		requested_channel = bot.channel_get_sync(parameter);
		if (!requested_channel.is_voice_channel()) {
			embed_reason = event.command.usr.get_mention() + " tried to invite cringe to VC, but provided a non-voice channel.";
			embed = status_embed("CringeError::VoiceError", embed_reason, Cringe::CringeStatus::ERROR);
			dpp::message message(event.command.channel_id, embed);
			event.reply(message);
			return;
		}

		auto channel = event.from->get_voice(event.command.guild_id);

		if (channel) {
			auto users_vc = guild->voice_members.find(bot.me.id);
			if (users_vc != guild->voice_members.end())
				if (channel->channel_id == users_vc->second.channel_id) {
					// Issue warning that cringe must be called to a channel that it is not already in
					embed_reason = event.command.usr.get_mention() + " tried to invite cringe to VC, but it is already there.";
					embed = status_embed("CringeError::VoiceWarning", embed_reason, Cringe::CringeStatus::WARNING);
					dpp::message message(event.command.channel_id, embed);
					event.reply(message);
					return;
				}
			event.from->disconnect_voice(event.command.guild_id);
		}
		event.from->connect_voice(guild->id, requested_channel.id, false, true);
		embed_reason = event.command.usr.get_mention() + " asked cringe to join " + requested_channel.get_mention();
		embed = status_embed("Successfully Joined", embed_reason, Cringe::CringeStatus::SUCCESS);
		dpp::message message(event.command.channel_id, embed);
		event.reply(message);
		return;
	}

	// Get the voice channel that cringe is currently in from this server (null if cringe not in vc)
	auto channel = event.from->get_voice(event.command.guild_id);

	// If cringe currently in VC, see which channel cringe is in
	if (channel) {
		// Find the channel that the issuing user is in
		auto users_vc = guild->voice_members.find(event.command.get_issuing_user().id);
		// Check if issuing user is trying to add cringe to channel it is already in
		if (users_vc != guild->voice_members.end() && channel->channel_id == users_vc->second.channel_id) {
			// Issue warning that cringe must be called to a channel that it is not already in
			embed_reason = event.command.usr.get_mention() + " tried to invite cringe to VC, but it is already there.";
			embed = status_embed("CringeError::VoiceWarning", embed_reason, Cringe::CringeStatus::WARNING);
			dpp::message message(event.command.channel_id, embed);
			event.reply(message);
			return;
		}
		// At this point, cringe is being called into a channel that it is currently not in (although it is in another channel)
		event.from->disconnect_voice(event.command.guild_id);
	}


	// Check to see if the issuing user is not in a voice channel and issue error if they did not provide an option of where to go
	if (!guild->connect_member_voice(event.command.get_issuing_user().id)) {
		embed_reason = "You must be in a VC, or specify a channel if you wish to invite cringe to VC.";
		embed = status_embed("CringeError::VoiceError", embed_reason, Cringe::CringeStatus::ERROR);
		dpp::message message(event.command.channel_id, embed);
		event.reply(message);
		return;
	}

	// At this point, cringe has successfully joined the VC, and a success message can be issued
	embed_reason = event.command.usr.get_mention() + " asked cringe to join.";
	embed = status_embed("Successfully Joined", embed_reason, Cringe::CringeStatus::SUCCESS);
	dpp::message message(event.command.channel_id, embed);
	event.reply(message);
}

int encode_audio_to_opus(const int16_t *pcm_data, int frame_size, unsigned char *opus_data, int max_data_size, OpusEncoder *encoder) {
	// Encode audio to OPUS
	return opus_encode(encoder, pcm_data, frame_size, opus_data, max_data_size);
}

bool determine_if_url(const std::string &query) {
	std::cout << "PARSING THROUGH SONG" << std::endl;
	// Regular expression to determine if a given url is from youtube
	std::regex youtube_regex(
			R"(^(?:(?:https?:)?\/\/)?(?:www\.)?(?:youtube\.com\/(?:[^\/\n\s]+\/\S+\/|(?:v|e(?:mbed)?)\/|\S*?[?&]v=)|youtu\.be\/)([a-zA-Z0-9_-]{11}))");
	// Match the user query against the regular expression definition
	bool is_youtube_link = std::regex_match(query, youtube_regex);
	std::cout << "DONE PARSING THROUGH SONG" << std::endl;
	return (is_youtube_link);
}

std::string get_youtube_elements(const char *cmd) {
	// Get the youtube video thumbnail, author info, and song info
	FILE *pipe = popen(cmd, "r");
	// Check that the pipe was opened successfully
	if (!pipe) {
		std::cerr << "Error opening pipe" << std::endl;
	}
	// Allocate c style buf to store result of command
	char buffer[128];
	std::string result;
	// Write contents of stdout buf to c++ style string
	while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
		result += buffer;
	}
	// Return a string of elements delimited by a newline
	return result;
}

std::vector<std::string> split_string(const std::string &input, char delimiter) {
	std::vector<std::string> tokens;
	std::istringstream stream(input);
	std::string token;

	while (std::getline(stream, token, delimiter)) {
		tokens.push_back(token);
	}

	return tokens;
}


void play_command(const dpp::cluster &bot, const dpp::slashcommand_t &event) {

	std::string embed_reason;
	dpp::embed embed;
	dpp::guild *guild;
	dpp::snowflake parameter;
	dpp::channel requested_channel;

	dpp::guild *g = dpp::find_guild(event.command.guild_id);

	/* Get the voice channel the bot is in, in this current guild. */
	dpp::voiceconn *v = event.from->get_voice(event.command.guild_id);

	/* If the voice channel was invalid, or there is an issue with it, then tell the user. */
	if (!v || !v->voiceclient || !v->voiceclient->is_ready()) {
		dpp::message message(event.command.channel_id, status_embed("CringeError::VoiceError", "Bot was unable to join the voice channel due to some unknown reason.", Cringe::CringeStatus::ERROR));
		event.reply(message);
		return;
	}

	// We need to set this otherwise the bot attempts to send all packets at once
	// Note: This is expensive!!! Might need to figure out another way...
	v->voiceclient->set_send_audio_type(dpp::discord_voice_client::satype_overlap_audio);

	// Get the song that the user wishes to play
	std::string song = std::get<std::string>(event.get_parameter("song"));

	std::cout << song << std::endl;

	// Set the bot to thinking. This gives us a bit more time to reply to the interaction
	event.thinking();

//	avformat_network_init();
//	std::string url = "https://www.youtube.com/watch?v=MC7qoiJ5uPc";
//
//	const char* cmd = R"(yt-dlp -g -f bestaudio --user-agent "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36" "https://www.youtube.com/watch?v=MC7qoiJ5uPc")";
//	FILE* pipe = popen(cmd, "r");
//
//	char buffer[128];
//	std::string audio_stream_url;
//
//	if (!pipe) {
//		std::cerr << "Error opening pipe to youtube-dl" << std::endl;
//	}
//
//	while (fgets(buffer, 128, pipe) != nullptr) {
//		audio_stream_url += buffer;
//	}
//
//	int res = pclose(pipe);
//
//	if (res == -1) {
//		perror("Error closing pipe to youtube-dl");
//	} else {
//		// Check if the pipe was successfully closed
//		if (WIFEXITED(res)) {
//			int exit_status = WEXITSTATUS(res);
//			if (exit_status != 0) {
//				std::cerr << "youtube-dl process exited with non-zero status: " << exit_status << std::endl;
//			}
//		} else if (WIFSIGNALED(res)) {
//			int signal_number = WTERMSIG(res);
//			std::cerr << "youtube-dl process terminated by signal: " << signal_number << std::endl;
//		} else if (WIFSIGNALED(res) == 0 && WIFEXITED(res) == 0) {
//			std::cerr << "Error closing pipe to youtube-dl: No child processes" << std::endl;
//		} else {
//			std::cerr << "Unknown exit status or termination signal from youtube-dl process" << std::endl;
//		}
//	}
//
//	// Remove newline characters from the end of the URL
//	audio_stream_url.erase(std::remove(audio_stream_url.begin(), audio_stream_url.end(), '\n'), audio_stream_url.end());
//
//	AVFormatContext* formatContext = avformat_alloc_context();
//	if (avformat_open_input(&formatContext, audio_stream_url.c_str(), nullptr, nullptr) != 0) {
//		std::cerr << "Error opening input file with FFmpeg" << std::endl;
//	}
//
//	if (avformat_find_stream_info(formatContext, nullptr) < 0) {
//		std::cerr << "Error finding stream information" << std::endl;
//		avformat_close_input(&formatContext);
//	}
//
//	std::cout << formatContext->audio_codec->id << std::endl;
//
//	std::cout << formatContext->duration << std::endl;
//
//	int audioStreamIndex = -1;
//
//	for (int i = 0; i < formatContext->nb_streams; ++i) {
//		if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
//			std::cout << formatContext->streams[i]->codecpar->format << std::endl;
//			audioStreamIndex = i;
//			break;
//		}
//	}
//
//	if (audioStreamIndex == -1) {
//		std::cerr << "No audio stream found" << std::endl;
//		avformat_close_input(&formatContext);
//	}
//
//	AVCodecParameters* codecParameters = formatContext->streams[audioStreamIndex]->codecpar;
//	std::cout << codecParameters << std::endl;
//	auto* codec = avcodec_find_decoder(codecParameters->codec_id);
//	if (codec == nullptr) {
//		std::cerr << "Unsupported codec" << std::endl;
//		avformat_close_input(&formatContext);
//	}
//
//	Allocate a codec context for the decoder
//	AVCodecContext* codecContext = avcodec_alloc_context3(codec);
//	if (avcodec_parameters_to_context(codecContext, codecParameters) < 0) {
//		std::cerr << "Failed to copy codec parameters to codec context" << std::endl;
//		avformat_close_input(&formatContext);
//		avcodec_free_context(&codecContext);
//	}
//
//	std::cout << codecContext->codec->long_name << std::endl;
//
//	// Open the codec
//	if (avcodec_open2(codecContext, codec, nullptr) != 0) {
//		std::cerr << "Failed to open codec" << std::endl;
//		avformat_close_input(&formatContext);
//		avcodec_free_context(&codecContext);
//	}
//
//	std::cout << codecContext->codec->long_name << std::endl;
//	std::cout << formatContext->audio_codec << std::endl;
//
//	AVPacket packet;
//	av_packet_unref(&packet);
//	av_init_packet(&packet);
//
//	AVFrame* frame = av_frame_alloc();
//
	// OpusEncoder* encoder = opus_encoder_create(48000, 2, 2049, nullptr);

	// Set the bitrate for OPUS encoding (adjust according to your needs)
	// opus_encoder_ctl(encoder, OPUS_SET_BITRATE(48000));

	// Remove newline characters from the
	song.erase(std::remove(song.begin(), song.end(), '\n'), song.end());

	// First determine if the song is not a URL. Currently, we can only get URLs, but we will add the option for queries in the future.
	if (!determine_if_url(song)) {
		// Issue an error if the query is not a URL
		embed_reason = "cringe can only parse through URL's to youtube at the moment :) Give it some time!";
		embed = status_embed("CringeError::VoiceError", embed_reason, Cringe::CringeStatus::ERROR);
		dpp::message message(event.command.channel_id, embed);
		event.edit_original_response(message);
		return;
	}

	std::cout << song << std::endl;

	// Set the url and codec, piping the audio with ffmpeg
	std::string song_streamer = "yt-dlp -f bestaudio -q --ignore-errors -o - \"" + song +
								"\" | ffmpeg -i pipe:0 -f s16le -ac 2 -ar 48000 pipe:1";
	// -loglevel quiet
	const char *audio_codec = song_streamer.c_str();

	// buf to store contents
	std::byte buf[11520];

	// string that gets all the information about the song
	std::string cmd = "yt-dlp --print artist --print title --print upload_date --print duration --print thumbnail \"" + song + "\"";

	// Extract information for the embed
	std::string output = get_youtube_elements(cmd.c_str());

	// Print the captured output
	std::cout << "Command Output:\n" << output << std::endl;

	// Split the output into lines
	std::vector<std::string> lines = split_string(output, '\n');

	// Process each line
	for (const auto &line: lines) {
		std::cout << "Element: " << line << std::endl;
	}

	std::cout << audio_codec << std::endl;

	// Subprocess fork that runs our ffmpeg piped audio
	auto pipe = popen(audio_codec, "r");

	// Check if pipe was unsuccessful
	if (!pipe) {
		std::cout << "Failed to open Pipe" << std::endl;
		return;
	}

	// Embed letting user know that the bot is playing
	embed = playing_embed(Cringe::CringeStatus::SUCCESS);
	dpp::message message(event.command.channel_id, embed);
	event.edit_original_response(message);

	// Bytes from output
	size_t bytes_read;

	// Get audio from song and pipe to discord
	while ((bytes_read = fread(buf, sizeof(std::byte), 11520, pipe)) > 0) {
		if (bytes_read < 11520) {
			std::cout << "11520 was bigger than bytes read: " << bytes_read << std::endl;
			continue;
		}
		// Send audio data if cringe is in an on-ready state
		if (v && v->voiceclient && v->voiceclient->is_ready()) {
			v->voiceclient->send_audio_raw((uint16_t *) buf, sizeof(buf));
		} else {
			std::cout << "VoiceClient was not ready" << std::endl;
		}
	}
	std::cout << "Reached the end of stream" << std::endl;
	pclose(pipe);
}

