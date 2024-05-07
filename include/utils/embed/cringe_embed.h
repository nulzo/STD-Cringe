#ifndef CRINGE_CRINGE_EMBED_H
#define CRINGE_CRINGE_EMBED_H

#include <dpp/dpp.h>
#include "utils/misc/cringe.h"
#include "utils/audio/cringe_song.h"

using json = nlohmann::json;

dpp::embed now_streaming(CringeSong song);
dpp::embed info_embed(const std::string& title, const std::string& response, const std::string& avatar_url, const std::string& mention, const std::string& created, const std::string& joined_at, const std::string& premium, const std::string& nitro, const std::string& bot);
dpp::embed added_to_queue_embed(CringeSong song);
dpp::embed confession_embed(const std::string& confession);
dpp::embed reddit_embed(nlohmann::json data);

class CringeEmbed {
public:
	CringeEmbed();
	~CringeEmbed();
	dpp::embed embed;
	int color = Cringe::CringeColor::CringePrimary;
	std::string icon = Cringe::CringeIcon::TerminalIcon;
	std::string title = "Cringe Embed";
	std::string profile_pic = "https://cdn.discordapp.com/avatars/1186860332845629511/2b20f3636a5bd288bca2eb197badf556.png";
	std::string help = "use a command with slashcommands!";
	CringeEmbed& setTitle(const std::string &embed_title);
	CringeEmbed& setIcon(const std::string &icon);
	CringeEmbed& setColor(const int &color);
	CringeEmbed& setFields(const std::vector<std::vector<std::string>> &fields);
	CringeEmbed& setHelp(const std::string &help_text);
	CringeEmbed& setDescription(const std::string &embed_description);
	CringeEmbed& setImage(const std::string &embed_image);
};

CringeEmbed cringe_success_embed(std::string &reason);
CringeEmbed cringe_warning_embed(std::string &reason);
CringeEmbed cringe_error_embed(std::string &reason);

#endif
