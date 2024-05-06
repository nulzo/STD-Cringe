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

#include "utils/audio/cringe_audio_helpers.h"

#include <fmt/format.h>

#include <algorithm>
#include <regex>

#include "sstream"

bool is_url(const std::string &query) {
    // Regular expression to determine if a given query is a URL
    std::regex url_regex(R"(^(https?|ftp):\/\/[^\s/$.?#].[^\s]*)");
    // Match the user query against the regular expression definition
    return std::regex_match(query, url_regex);
}

std::vector<std::string> get_yt_info(std::string song) {
    // Allocate c style buf to store result of command
    char buffer[128];
    std::string result;
    std::vector<std::string> yt_data;
    std::string data;
    // string that gets all the information about the song
    std::string cmd =
        fmt::format("yt-dlp -s --print title --print channel --print thumbnail --print duration --print view_count --print comment_count --print epoch --print channel_follower_count \"{}\"", song);
    // Get the YouTube video thumbnail, author info, and song info
    FILE *pipe = popen(cmd.c_str(), "r");
    // Check that the pipe was opened successfully
    if (!pipe) {
        std::cerr << "Error opening pipe" << std::endl;
    }
    // Write contents of stdout buf to c++ style string
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    std::istringstream stream(result);
    // Loop through line and add to vector
    while (std::getline(stream, data, '\n')) {
        yt_data.push_back(data);
    }
    // Returns a vector with the elements <TITLE, ARTIST, THUMB_URL, DURATION,
    // VIEWS, COMMENTS, UPLOADDATE, SUBSCRIBERS>
    return yt_data;
}

std::string search_command(std::string search) {
    return fmt::format(R"(yt-dlp -f bestaudio -o - -vn "{}" | ffmpeg -i pipe:0 -hide_banner -loglevel warning -f s16le -ac 2 -ar 48000 pipe:1)", search);
}

std::string query_to_url(std::string query) {
    // Allocate c style buf to store result of command
    char buffer[128];
    // Var to store processed URL
    std::string URL;
    // The query to get the YouTube URL
    std::string command =
        fmt::format("yt-dlp --print webpage_url \"ytsearch:{}\"", query);
    // Convert to a C-string
    const char *cmd = command.c_str();
    // Open the pipe to process to command
    FILE *pipe = popen(cmd, "r");
    // Write contents of stdout buf to c++ style string
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        URL += buffer;
    }
    // Return the URL
    return URL;
}

std::string sanitize_query(std::string query) {
    query.erase(std::remove(query.begin(), query.end(), '\n'), query.end());
    query.erase(std::remove(query.begin(), query.end(), '"'), query.end());
    query.erase(std::remove(query.begin(), query.end(), '{'), query.end());
    query.erase(std::remove(query.begin(), query.end(), '}'), query.end());
    return query;
}
