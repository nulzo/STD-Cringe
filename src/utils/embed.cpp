#include "utils/embed.h"
#include "utils/cringe.h"

dpp::embed status_embed(const std::string &title, const std::string &reason, int status) {
	dpp::embed embed = dpp::embed();
	switch (status) {
		case Cringe::CringeStatus::ERROR:
			embed.set_color(Cringe::CringeColor::CringeError).set_thumbnail(Cringe::CringeIcon::ErrorIcon).set_description("An error has occurred due to the following exceptions");
			break;
		case Cringe::CringeStatus::WARNING:
			embed.set_color(Cringe::CringeColor::CringeWarning).set_thumbnail(Cringe::CringeIcon::WarningIcon).set_description("A warning has occurred due to the following exceptions");
			break;
		case Cringe::CringeStatus::SUCCESS:
			embed.set_color(Cringe::CringeColor::CringeSuccess).set_thumbnail(Cringe::CringeIcon::SuccessIcon).set_description("A success has been issued due to the following");
			break;
	}
	return embed.set_title(title).add_field("Reason", reason).set_timestamp(time(nullptr));
}

dpp::embed playing_embed(int status) {
	dpp::embed embed;
	embed.set_color(Cringe::CringeColor::CringeOrange).set_thumbnail(Cringe::CringeIcon::MusicIcon).set_timestamp(time(nullptr));
	embed.set_title("Now Streaming").set_description("Now streaming to your channel!").set_timestamp(time(nullptr));
	return embed;
}

dpp::embed generic_embed(const std::string &icon, const std::string &title, const std::string &description, int color){
	dpp::embed embed;
	embed.set_color(color);
	embed.set_description(description);
	embed.set_thumbnail(icon);
	embed.set_title(title);
	embed.set_timestamp(time(nullptr));
	return embed;
}
