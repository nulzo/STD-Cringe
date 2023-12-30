#ifndef CRINGE_REDDITCOMMAND_H
#define CRINGE_REDDITCOMMAND_H

#include <dpp/dpp.h>

dpp::slashcommand reddit_declaration();
void reddit_command(dpp::cluster &bot, const dpp::slashcommand_t &event);

#endif //CRINGE_REDDITCOMMAND_H
