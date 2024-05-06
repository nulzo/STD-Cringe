#include <curl/curl.h>

#include "utils/misc/cringe.h"
#include "utils/misc/cringe_helpers.h"

size_t cringe_callback(void *contents, size_t size, size_t nmemb,
                       std::string *output) {
    size_t totalSize = size * nmemb;
    output->append(static_cast<char *>(contents), totalSize);
    return totalSize;
}

json cringe_post(const std::string &data, const std::string &url,
                 const std::vector<std::string> &header = {}) {
    // Initialize libcurl
    CURL *curl;
    CURLcode response_code;
    json response;
    std::string response_data;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        // Set the headers
        struct curl_slist *headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        // If the user passed in headers, iterate through and assign them to the
        // linked list
        if (!header.empty()) {
            for (const std::string &head : header) {
                headers = curl_slist_append(headers, head.c_str());
            }
        }
        // Set the callback function to handle the response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cringe_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
        // Perform the GET request
        response_code = curl_easy_perform(curl);
        if (response_code == CURLE_OK) {
            curl_easy_cleanup(curl);
            curl_slist_free_all(headers);
            curl_global_cleanup();
            return json::parse(response_data);
        } else {
            std::cerr << "cURL request failed: "
                      << curl_easy_strerror(response_code) << std::endl;
            curl_easy_cleanup(curl);
            curl_slist_free_all(headers);
            curl_global_cleanup();
            return json::parse(
                fmt::format("error: {}", curl_easy_strerror(response_code)));
        }
    }
    // Clean up libcurl
    curl_global_cleanup();
    return json::parse(
        fmt::format("error: unknown error occurred in making API request!"));
}
