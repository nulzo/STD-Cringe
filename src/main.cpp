/*
 * MIT License
 *
 * Copyright (c) 2023 @nulzo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "utils/bot/cringe_bot.h"

struct Config {
    std::string token;
    std::string guild;
};

int main(int argc, char *argv[]) {
    /* Main entrance for the Cringe bot. Several command line arguments can be
     * given with the following descriptions:
     * - token (required): The discord token to run the bot.
     * - guild (optional): The guild you want the bot to be accessed in.
     * */

    std::unordered_map<std::string, std::string> args;
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg.substr(0, 2) == "--") {
            size_t equalsPos = arg.find('=');
            if (equalsPos != std::string::npos) {
                std::string name = arg.substr(2, equalsPos - 2);
                std::string value = arg.substr(equalsPos + 1);
                args[name] = value;
            }
        }
    }

    auto token = args.find("token");
    if (token == args.end()) {
        std::cerr << "Must provide bot token!" << std::endl;
        return -1;
    }

    CringeBot cringe(token->second);
    return 0;
}
