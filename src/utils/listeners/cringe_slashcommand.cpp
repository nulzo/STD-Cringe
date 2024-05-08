#include <functional>

#include "commands/api/reddit_command.h"
#include "commands/chat/chat_command.h"
#include "commands/chat/code_command.h"
#include "commands/chat/confession_command.h"
#include "commands/chat/describe_command.h"
#include "commands/chat/imagine_command.h"
#include "commands/chat/info_command.h"
#include "commands/chat/user_command.h"
#include "commands/voice/join_command.h"
#include "commands/voice/play_command.h"
#include "commands/voice/queue_command.h"
#include "commands/voice/skip_command.h"
#include "utils/bot/cringe_bot.h"


void process_slashcommand(const dpp::slashcommand_t &event,  CringeBot &cringe) {
	if (event.command.get_command_name() == "info") {
		info_command(cringe, event);
	}
	else if (event.command.get_command_name() == "user") {
		user_command(cringe, event);
	}
	else if (event.command.get_command_name() == "chat") {
		std::thread chat(chat_command, std::ref(cringe), event);
		chat.detach();
	}
	else if (event.command.get_command_name() == "code") {
		std::thread code(code_command, std::ref(cringe), event);
		code.detach();
	}
	else if (event.command.get_command_name() == "join") {
		join_command(cringe, event);
	}
	else if (event.command.get_command_name() == "play") {
		std::thread voice(play_command, std::ref(cringe), event);
		voice.detach();
	}
	else if (event.command.get_command_name() == "queue") {
		queue_command(event, cringe.queue);
	}
	else if (event.command.get_command_name() == "skip") {
		skip_command(cringe, event);
	}
	else if (event.command.get_command_name() == "confess") {
		confession_command(cringe, event);
	}
	else if (event.command.get_command_name() == "reddit") {
		reddit_command(cringe, event);
	}
	else if (event.command.get_command_name() == "imagine") {
		std::thread imagine(image_command, std::ref(cringe), event);
		imagine.detach();
	}
	else if(event.command.get_command_name() == "describe") {
		std::thread describe(describe_command, std::ref(cringe), event);
		describe.detach();
	}
}

void register_slashcommands(CringeBot &cringe) {
    std::vector<dpp::slashcommand> commands{{
        info_declaration(),
        user_declaration(),
        chat_declaration(cringe),
        join_declaration(),
        code_declaration(),
        play_declaration(),
        queue_declaration(),
        skip_declaration(),
        confession_declaration(),
        reddit_declaration(),
        image_declaration(),
        describe_declaration(),
    }};
    cringe.cluster.global_bulk_command_create(commands);
}
