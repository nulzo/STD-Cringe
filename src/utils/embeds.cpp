#include "embeds.h"

dpp::embed
error_embed(const std::string &title, const std::string &description, const std::string &reason) {
	return dpp::embed()
			.set_color(color::CringeError)
			.set_title(title)
			.set_description(description)
			.set_thumbnail("https://i.imgur.com/L9qbxDB.png")
			.add_field("Reason", reason)
			.set_timestamp(0);
}

dpp::embed success_embed(const std::string &title, const std::string &description, const std::string &reason) {
	return dpp::embed()
			.set_color(color::CringeSuccess)
			.set_title(title)
			.set_description(description)
			.set_thumbnail(CringeIcon::SuccessIcon)
			.add_field("Reason", reason)
			.set_timestamp(0);
}

