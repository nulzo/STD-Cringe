#ifndef CRINGE_CODE_COMMAND_H
#define CRINGE_CODE_COMMAND_H

#include <dpp/dpp.h>
#include <dpp/unicode_emoji.h>

dpp::slashcommand code_declaration();
void code_command(dpp::cluster &bot, const dpp::slashcommand_t &event);


#endif //CRINGE_CODE_COMMAND_H
