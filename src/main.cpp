#include "utils/utils.h"
#include "commands/commands.h"

int main() {
    std::string BOT_TOKEN;
    get_env("BOT_TOKEN", BOT_TOKEN);

    dpp::cluster bot(BOT_TOKEN, dpp::i_default_intents | dpp::i_message_content);
    bot.on_log(dpp::utility::cout_logger());

    bot.on_slashcommand([&bot](const dpp::slashcommand_t &event) {
        if (event.command.get_command_name() == "info") {
            info_command(bot, event);
        } else if (event.command.get_command_name() == "problem") {
            problem_command(bot, event);
        } else if (event.command.get_command_name() == "user") {
            user_command(bot, event);
        } else if (event.command.get_command_name() == "chat") {
            chat_command(bot, event);
        } else if (event.command.get_command_name() == "join") {
            join_command(bot, event);
        } else if (event.command.get_command_name() == "play") {
            play_command(bot, event);
        }
    });

    bot.on_ready([&bot](const dpp::ready_t &event) {
        /* Create a new global command on ready event */
        if (dpp::run_once<struct register_bot_commands>()) {
            std::vector<dpp::slashcommand> commands{
                    {
                            info_declaration(),
                            user_declaration(),
                            problem_declaration(),
                            chat_declaration(),
                            join_declaration(),
                            play_declaration()
                    }
            };
            bot.global_bulk_command_create(commands);
            bot.set_presence(dpp::presence(dpp::ps_online, dpp::at_custom, "made by @nulzo"));
        }
    });

    bot.start(dpp::st_wait);
    return 0;
}