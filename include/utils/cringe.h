#ifndef CRINGE_CRINGE_H
#define CRINGE_CRINGE_H

#include <queue>
#include "iostream"

namespace Cringe {

	namespace CringeStatus {
		const int ERROR = 1;
		const int WARNING = 2;
		const int SUCCESS = 3;
		const int NEWSONG = 4;
		const int CURRSONG = 5;
	}

	namespace CringeIcon {
		const std::string SuccessIcon = "https://i.imgur.com/rpSPSCk.png";
		const std::string ErrorIcon = "https://i.imgur.com/rFiaIWw.png";
		const std::string WarningIcon = "https://i.imgur.com/1lfJlup.png";
		const std::string MusicIcon = "https://i.imgur.com/CVJNkN3.png";
	}

	namespace CringeColor {
		// Cringe Brand Color
		const int CringeOrange = 0xFF7C20;
		// Cringe Status Colors
		const int CringeError = 0xEF5350;
		const int CringeSuccess = 0x10b981;
		const int CringeWarning = 0xFBBF24;
		const int CringeInfo = 0x06B6D4;
		// Cringe Standard Colors
		const int CringeWhite = 0xF1F5F9;
		const int CringeBlue = 0x7391C8;
		const int CringePurple = 0x8B5CF6;
		const int CringeIndigo = 0x6366F1;
		const int CringePink = 0xEC4899;
		const int CringeBlack = 0x1E2124;
	}

	class CringeAudio {
	public:
		static std::vector<std::string> get_yt_info(std::string song);

		static bool is_yt_url(const std::string &query);

		static std::string get_encoded_url(std::string song);

		int codec_to_opus(const int16_t *pcm_data, int frame_size, unsigned char *opus_data, int max_data_size, OpusEncoder *encoder);
	};

	class CringeSong {
	public:
		CringeSong(std::string title, std::string artist, std::string thumbnail, std::string duration);

		std::string get_title();

		void set_title(std::string t);

		std::string get_artist();

		void set_artist(std::string a);

		std::string get_thumbnail();

		void set_thumbnail(std::string tn);

		std::string get_formatted_duration();

		void set_formatted_duration(std::string fd);

	private:
		std::string title;
		std::string artist;
		std::string thumbnail;
		std::string duration;
	};

	class CringeQueue {
		int length;
	public:
		[[nodiscard]] bool is_empty() const;

		void enqueue(const Cringe::CringeSong &song);

		void dequeue();

		std::string peek();

		void display_queue() const;

	private:
		std::queue<CringeSong> cringe_queue;
	};

	class CringeBot {

	};
}

#endif //CRINGE_CRINGE_H
