#include "utils.h"

int get_request(const char *request, const char *URL, const char *auth_header, std::string &response) {
    CURL *curl;
    CURLcode response_code;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        struct curl_slist *headers = nullptr;

        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, auth_header);

        curl_easy_setopt(curl, CURLOPT_URL, URL);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

        // Set the response data variable as the cURL write callback data
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Perform the HTTP request
        response_code = curl_easy_perform(curl);

        if (response_code == CURLE_OK) {
            curl_easy_cleanup(curl);
            curl_slist_free_all(headers);
            curl_global_cleanup();
            return 0;
        } else {
            std::cerr << "cURL request failed: " << curl_easy_strerror(response_code) << std::endl;
            return -1;
        }
    }
    return -1;
}

std::string open_ai_api(const std::string &prompt, std::string max_tokens, std::string model) {
    const char *request;
    int return_value;
    std::string response;
    std::string url;
    std::string auth;
    std::string json_payload;
    std::string system_role;
    json responseJson;

    // Load in env info
    get_env("OPEN_AI_AUTH", auth);
    get_env("OPEN_AI_ENDPOINT", url);
    get_env("SYSTEM_ROLE", system_role);
    auth = fmt::format("Authorization: Bearer {}", auth);

    // To c_strs for cURL
    const char *URL = url.c_str();
    const char *AUTH = auth.c_str();

    // Do this on this line and split up the call to .c_str(), otherwise we can accidentally leave a dangling pointer
    json_payload = fmt::format(
            R"({{"model": "{}","messages": [{{"role": "system","content": "{}" }},{{"role": "user", "content": "{}" }}],"temperature": 1,"max_tokens": {}, "top_p": 1,"frequency_penalty": 0,"presence_penalty": 0 }})",
            model, system_role, prompt, max_tokens);

    // Set the request body to a c_str since that is what cURL expects
    request = json_payload.c_str();

    // Call our function
    return_value = get_request(request, URL, AUTH, response);

    std::cout << response;

    if (return_value != 0) {
        std::cerr << "ERROR: request failed! Falling back with error!" << std::endl;
        return "There was an error with the request";
    }
    responseJson = json::parse(response);
    return responseJson["choices"][0]["message"]["content"];
}

std::string discord_time_to_date(double timestamp) {
    // Set the epoch time for Jan 1, 2015
    const time_t epoch = 1420070400; // January 1, 2015, 00:00:00 (GMT)
    // Calculate the elapsed time in seconds
    auto elapsedTime = static_cast<time_t>(timestamp - epoch);
    // Convert to tm structure
    struct tm *timeinfo = std::localtime(&elapsedTime);
    // Format the date as dd/mm/yyyy
    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%d/%m/%Y", timeinfo);
    return buffer;
}

size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *output) {
    size_t totalSize = size * nmemb;
    output->append(static_cast<char *>(contents), totalSize);
    return totalSize;
}

int get_env(std::string given_key, std::string &return_value) {
    std::string line;
    std::smatch matches;
    std::string key;
    std::string value;
    std::ifstream file(".env");
    // Open the .env file and error if can't open
    if (!file.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return -1;
    }
    // Read and parse the file line by line
    while (std::getline(file, line)) {
        // Regex to split on equal sign
        std::regex pattern("([^=]+)=(.+)");
        // Match the input against the regex pattern
        if (std::regex_match(line, matches, pattern)) {
            // Extract values from the regex matching
            key = matches[1];
            value = matches[2];
            // Check to see if the key matches what we're looking for
            if (key == given_key) {
                return_value = value;
                // Close the file
                file.close();
                return 0;
            }
        }
    }
    // Close the file
    file.close();
    // No match
    return -1;
}
