//
// Created by Nolan Gregory on 2/17/24.
//

#ifndef CRINGE_DESCRIBECOMMAND_H
#define CRINGE_DESCRIBECOMMAND_H

#include <dpp/dpp.h>
#include <dpp/unicode_emoji.h>

dpp::slashcommand describe_declaration();
void describe_command(dpp::cluster &bot, const dpp::slashcommand_t &event);


#endif //CRINGE_DESCRIBECOMMAND_H
