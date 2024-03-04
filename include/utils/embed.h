#pragma once

#ifndef CRINGE_EMBEDS_H
#define CRINGE_EMBEDS_H

#include <dpp/dpp.h>
#include "cringe.h"

dpp::embed status_embed(const std::string &title, const std::string &reason, int status);
dpp::embed playing_embed(int status);
dpp::embed now_streaming(Cringe::CringeSong song);
dpp::embed info_embed(const std::string& title, const std::string& response, const std::string& avatar_url, const std::string& mention, const std::string& created, const std::string& joined_at, const std::string& premium, const std::string& nitro, const std::string& bot);
dpp::embed generic_embed(const std::string &icon, const std::string &title, const std::string &description, int status);
dpp::embed added_to_queue_embed(Cringe::CringeSong song);
dpp::embed confession_embed(const std::string& confession);
dpp::embed chat_embed(std::string &prompt, std::string &response, const dpp::slashcommand_t &event);
dpp::embed imagine_embed(const std::string &prompt, const std::string &image_file, const dpp::slashcommand_t &event);
dpp::embed describe_embed(const std::string &response, const dpp::attachment& image, const dpp::slashcommand_t &event);
dpp::embed reddit_embed(nlohmann::json data);
dpp::embed cringe_embed(
		const std::string &title,
		const std::string &description,
		const std::string &tooltip,
		const std::string &bot_avatar,
		const std::string &icon,
		const int &color,
		const std::vector<std::vector<std::string>>& fields
);

class CringeEmbed {
private:
	std::string title = "Cringe Embed";
	std::string description;
	std::string profile_pic = "https://cdn.discordapp.com/avatars/1186860332845629511/2b20f3636a5bd288bca2eb197badf556.png";
	int color = Cringe::CringeColor::CringePrimary;
	std::string icon = Cringe::CringeIcon::SuccessIcon;
	std::vector<std::vector<std::string>> fields;
	dpp::embed embed;
	std::string image;
	std::string help = "use a command with slashcommands!";
public:
	CringeEmbed& setTitle(const std::string &embed_title);
	CringeEmbed& setIcon(const std::string &icon);
	CringeEmbed& setHelp(const std::string &help_text);
	CringeEmbed& setDescription(const std::string &embed_description);
	CringeEmbed& initEmbed();
	dpp::embed getEmbed();
};

#endif //CRINGE_EMBEDS_H
