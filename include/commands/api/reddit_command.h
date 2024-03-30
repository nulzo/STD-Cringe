#ifndef CRINGE_REDDIT_COMMAND_H
#define CRINGE_REDDIT_COMMAND_H

#include <dpp/dpp.h>

dpp::slashcommand reddit_declaration();
void reddit_command(dpp::cluster &bot, const dpp::slashcommand_t &event);

#endif //CRINGE_REDDIT_COMMAND_H
