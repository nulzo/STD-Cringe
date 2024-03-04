#include "listeners/SlashcommandListener.h"
#include "commands/challenge/challenge.h"
#include "commands/misc/misc.h"
#include "commands/voice/PlayCommand.h"
#include "commands/voice/JoinCommand.h"
#include "commands/voice/QueueCommand.h"
#include "commands/voice/SkipCommand.h"
#include "commands/misc/ConfessionCommand.h"
#include "commands/api/RedditCommand.h"
#include "commands/api/ImageCommand.h"
#include "commands/api/TalkCommand.h"
#include "commands/api/ChatCommand.h"
#include "commands/api/UserCommand.h"
#include "commands/api/DescribeCommand.h"
#include "commands/api/CodeCommand.h"

void process_slashcommand(const dpp::slashcommand_t &event,  dpp::cluster &bot, Cringe::CringeQueue &queue) {
	if (event.command.get_command_name() == "info") {
		info_command(bot, event);
	}
	else if (event.command.get_command_name() == "problem") {
		problem_command(bot, event);
	}
	else if (event.command.get_command_name() == "user") {
		user_command(bot, event);
	}
	else if (event.command.get_command_name() == "chat") {
    std::thread chat(chat_command, std::ref(bot), event);
		chat.detach();
	}
  else if (event.command.get_command_name() == "code") {
    std::thread code(code_command, std::ref(bot), event);
    code.detach();
  }
	else if (event.command.get_command_name() == "join") {
		join_command(bot, event);
	}
	else if (event.command.get_command_name() == "play") {
		std::thread voice(play_command, std::ref(bot), event, std::ref(queue));
		voice.detach();
	}
	else if (event.command.get_command_name() == "message") {
		message_command(bot, event);
	}
	else if (event.command.get_command_name() == "ethan") {
		ethan_command(bot, event);
	}
	else if (event.command.get_command_name() == "queue") {
		queue_command(event, queue);
	}
	else if (event.command.get_command_name() == "skip") {
		skip_command(bot, event, queue);
	}
	else if (event.command.get_command_name() == "confess") {
		confession_command(bot, event);
	}
	else if (event.command.get_command_name() == "reddit") {
		reddit_command(bot, event);
	}
	else if (event.command.get_command_name() == "imagine") {
    std::thread imagine(image_command, std::ref(bot), event);
    imagine.detach();
	}
	else if(event.command.get_command_name() == "talk") {
		talk_command(bot, event);
	}
	else if(event.command.get_command_name() == "describe") {
    std::thread describe(describe_command, std::ref(bot), event);
    describe.detach();
	}
}

void register_slashcommands(dpp::cluster &bot) {
	std::vector<dpp::slashcommand> commands{
			{
					info_declaration(),
					user_declaration(),
					problem_declaration(),
					chat_declaration(),
					join_declaration(),
					play_declaration(),
					message_declaration(),
					ethan_declaration(),
					queue_declaration(),
					skip_declaration(),
					confession_declaration(),
					reddit_declaration(),
					image_declaration(),
					talk_declaration(),
					describe_declaration(),
          code_declaration()
    }
	};
	bot.global_bulk_command_create(commands);
	bot.set_presence(dpp::presence(dpp::ps_online, dpp::at_custom, "made by @nulzo"));
}
