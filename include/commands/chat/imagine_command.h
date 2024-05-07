#ifndef CRINGE_IMAGINE_COMMAND_H
#define CRINGE_IMAGINE_COMMAND_H

#include <dpp/dpp.h>
#include "utils/bot/cringe_bot.h"

dpp::slashcommand image_declaration();
void image_command(CringeBot &cringe, const dpp::slashcommand_t &event);

#endif
