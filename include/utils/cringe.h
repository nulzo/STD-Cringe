#ifndef CRINGE_CRINGE_H
#define CRINGE_CRINGE_H

#include <queue>
#include "iostream"
#include "dpp/dpp.h"

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
		const std::string ConfessionIcon = "https://i.imgur.com/Jd4rOxM.png";
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

		static std::string search_command(std::string search);

		static std::string sanitize_query(std::string query);

		static std::string query_to_url(std::string query);
	};

	class CringeSong {
	public:
		CringeSong(std::string title, std::string artist, std::string thumbnail, std::string duration, std::string url,
				   dpp::slashcommand_t &event);

		std::string get_title();

		std::string get_artist();

		std::string get_thumbnail();

		std::string get_formatted_duration();

		std::string get_url();

		dpp::slashcommand_t get_event();

		void set_title(std::string t);

		void set_artist(std::string a);

		void set_thumbnail(std::string tn);

		void set_formatted_duration(std::string fd);

		void set_url(std::string u);

		void set_event(dpp::slashcommand_t &e);

	private:
		std::string title;
		std::string artist;
		std::string thumbnail;
		std::string duration;
		std::string url;
		dpp::slashcommand_t event;
	};

	class CringeQueue {
		int length;
	public:
		[[nodiscard]] bool is_empty() const;

		void enqueue(const Cringe::CringeSong &song);

		Cringe::CringeSong dequeue();

		std::string peek();

		std::queue<CringeSong> get_queue();

		void display_queue() const;

	private:
		std::queue<CringeSong> cringe_queue;
	};

	class CringeBot {
		CringeSong song;
		CringeQueue queue;
		std::string BOT_TOKEN;
	};

	class CringeFilter {
	public:
		const std::string VAPORWAVE = "\"highpass=f=50, lowpass=f=2750, aresample=48000, asetrate=48000*0.85,bass=g=5:f=110:w=0.6, compand=attacks=0:points=-80/-169|-54/-80|-49.5/-64.6|-41.1/-41.1|-25.8/-15|-10.8/-4.5|0/0|20/8.3\"";
		const std::string INTHEBATHROOM = "\"highpass=f=10, lowpass=f=400, aresample=44100, asetrate=44100*0.85,bass=g=4:f=110:w=0.6, alimiter=1, compand=attacks=0:points=-80/-169|-54/-80|-49.5/-64.6|-41.1/-41.1|-25.8/-15|-10.8/-4.5|0/0|20/8.3\"";
		const std::string NIGHTCORE = "\"aresample=48000, asetrate=48000*1.5\"";
		const std::string BASSBOOST = "\"bass=g=14\"";
	};

}

#endif //CRINGE_CRINGE_H
