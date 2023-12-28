#ifndef CRINGE_CRINGE_H
#define CRINGE_CRINGE_H

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

	class CringeAudio{

	};
}

#endif //CRINGE_CRINGE_H
