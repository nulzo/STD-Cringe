#include "utils/misc/cringe_helpers.h"

#include "utils/misc/cringe.h"

void replace_raw_char(std::string &str, char refchar) {
    size_t found = str.find(refchar);
    while (found != std::string::npos) {
        std::string replace = fmt::format(R"(\{})", refchar);
        str.replace(found, 1, replace);
        found = str.find('\"', found + 2);
    }
}

std::string get_env(const std::string_view &given_key) {
    std::string line;
    std::string key;
    std::string value;
    std::ifstream file(".env");

    // Open the .env file and error if can't open
    if (!file.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return "";
    }

    // Read and parse the file line by line
    while (std::getline(file, line)) {
        // Find the position of the equal sign
        size_t equalPos = line.find('=');

        // Check if the line contains an equal sign and it is not the first
        // character
        if (equalPos != std::string::npos && equalPos > 0) {
            // Extract key and value based on the position of the equal sign
            key = line.substr(0, equalPos);
            value = line.substr(equalPos + 1);

            // Trim leading and trailing whitespaces from key and value
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);

            // Check to see if the key matches what we're looking for
            if (key == given_key) {
                // Close the file
                file.close();
                return value;
            }
        }
    }

    // Close the file
    file.close();

    // No match
    return "";
}

std::string seconds_to_formatted_time(int seconds) {
    int hours = seconds / 3600;
    int minutes = (seconds % 3600) / 60;
    int r_seconds = seconds % 60;
    return fmt::format("{:02}:{:02}:{:02}", hours, minutes, r_seconds);
}