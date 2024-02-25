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
	}

	namespace CringeIcon {
		const std::string SuccessIcon = "https://i.imgur.com/MLtOxom.png";
		const std::string ErrorIcon = "https://i.imgur.com/X57XzX7.png";
		const std::string WarningIcon = "https://i.imgur.com/6TA8UGp.png";
		const std::string MusicIcon = "https://i.imgur.com/OdDMcQc.png";
		const std::string ConfessionIcon = "https://i.imgur.com/mfQPfgd.png";
		const std::string LightningIcon = "https://i.imgur.com/LqMfdxQ.png";
		const std::string TerminalIcon = "https://i.imgur.com/tIdsnXA.png";
		const std::string ScrollIcon = "https://i.imgur.com/enf2hpR.png";
		const std::string ChatIcon = "https://i.imgur.com/COOT47N.png";
		const std::string AperatureIcon = "https://i.imgur.com/U33KuA6.png";
		const std::string InfoIcon = "https://i.imgur.com/2lZ8HBa.png";
	}

	namespace CringeEndpoint {
		const std::string Describe = "/api/v1/describe";
		const std::string Imagine = "/api/v1/generate";
		const std::string Chat = "/api/v1/chat";
	}

	namespace CringeColor {
		// Cringe Brand Color
		const int CringePrimary = 0xA69EE3;
		const int CringeSecondary = 0x445EBF;
		const int CringeDark = 0x201E30;
		const int CringeAccent = 0xC1A0D3;
		// Cringe Status Colors
		const int CringeError = 0xDC5968;
		const int CringeSuccess = 0x7EC99C;
		const int CringeWarning = 0xFCE0B1;
	}

	class CringeAudio {
	public:
		static std::vector<std::string> get_yt_info(std::string song);

		static bool is_url(const std::string &query);

		static std::string search_command(std::string search);

		static std::string sanitize_query(std::string query);

		static std::string query_to_url(std::string query);

		static void cringe_streamer(const std::string &ffmpeg_data, dpp::voiceconn *voice);
	};

	class CringeSong {
	public:
		CringeSong(std::string title, std::string artist, std::string thumbnail, std::string duration, std::string filter, std::string url, std::string upload_date, std::string comment_count,std::string subscriber_count, std::string view_count, dpp::slashcommand_t &event);

		std::string get_title();

		std::string get_artist();

		std::string get_thumbnail();

		std::string get_formatted_duration();

		std::string get_url();

		std::string get_filter();

		std::string get_subscribers();

		std::string get_view_count();

		std::string get_comments();

		dpp::slashcommand_t get_event();

		std::string get_upload_date();

	private:
		std::string title;
		std::string artist;
		std::string thumbnail;
		std::string duration;
		std::string url;
		std::string upload_date;
		std::string comment_count;
		std::string subscriber_count;
		std::string view_count;
		dpp::slashcommand_t event;
		std::string filter;
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
		const std::string BASSBOOST = "\"bass=g=20:f=110:w=0.3\"";
		const std::string DIM = "apulsator=hz=0.09";
		const std::string LOFI = "aresample=48000,asetrate=48000*0.9,extrastereo=m=2.5:c=disabled";
		const std::string NOSILENCE = "\"silenceremove=1:0:-50dB\"";
		const std::string EXPANDER = "\"compand=attacks=0:points=-80/-169|-54/-80|-49.5/-64.6|-41.1/-41.1|-25.8/-15|-10.8/-4.5|0/0|20/8.3\"";
	};
}

#endif //CRINGE_CRINGE_H
