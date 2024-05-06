#include "utils/bot/cringe_bot.h"
#include "commands/voice/play_command.h"
#include "commands/voice/join_command.h"
#include "commands/voice/queue_command.h"
#include "commands/voice/skip_command.h"
#include "commands/chat/confession_command.h"
#include "commands/api/reddit_command.h"
#include "commands/chat/imagine_command.h"
#include "commands/chat/chat_command.h"
#include "commands/chat/code_command.h"
#include "commands/chat/user_command.h"
#include "commands/chat/describe_command.h"
#include "commands/chat/info_command.h"
#include <functional>
#include <map>

using CommandHandler = std::function<void()>;

void process_slashcommand(const dpp::slashcommand_t &event, CringeBot &cringe) {
	static std::map<std::string, CommandHandler> handlers = {
		{"info", [&]() { info_command(cringe.cluster, event); }},
		{"user", [&]() { user_command(cringe.cluster, event); }},
		{"chat", [&]() { std::thread chat(chat_command, std::ref(cringe.cluster), event); chat.detach(); }},
		{"code", [&]() { std::thread code(code_command, std::ref(cringe.cluster), event); code.detach(); }},
		{"join", [&]() { join_command(cringe.cluster, event); }},
		{"play", [&]() { std::thread voice(play_command, std::ref(cringe.cluster), event, std::ref(cringe.queue)); voice.detach(); }},
		{"queue", [&]() { queue_command(event, cringe.queue); }},
		{"skip", [&]() { skip_command(cringe.cluster, event, cringe.queue); }},
		{"confess", [&]() { confession_command(cringe.cluster, event); }},
		{"reddit", [&]() { reddit_command(cringe.cluster, event); }},
		{"imagine", [&]() { std::thread imagine(image_command, std::ref(cringe.cluster), event); imagine.detach(); }},
		{"describe", [&]() { std::thread describe(describe_command, std::ref(cringe.cluster), event); describe.detach(); }}
	};
	auto command = handlers.find(event.command.get_command_name());
	if (command != handlers.end()) command->second();
}

void register_slashcommands(dpp::cluster &bot) {
	std::vector<dpp::slashcommand> commands{
		{
			info_declaration(),
			user_declaration(),
			chat_declaration(),
			join_declaration(),
			code_declaration(),
			play_declaration(),
			queue_declaration(),
			skip_declaration(),
			confession_declaration(),
			reddit_declaration(),
			image_declaration(),
			describe_declaration(),
    	}
	};
	bot.global_bulk_command_create(commands);
}
