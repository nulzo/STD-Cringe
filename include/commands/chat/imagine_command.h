//
// Created by Nolan Gregory on 1/1/24.
//

#ifndef CRINGE_IMAGINE_COMMAND_H
#define CRINGE_IMAGINE_COMMAND_H

#include <dpp/dpp.h>

dpp::slashcommand image_declaration();

void image_command(dpp::cluster &bot, const dpp::slashcommand_t &event);

#endif //CRINGE_IMAGINE_COMMAND_H
