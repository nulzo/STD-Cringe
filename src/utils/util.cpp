/*
 * MIT License
 *
 * Copyright (c) 2023 @nulzo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "utils/util.h"
#include "utils/cringe.h"

size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *output) {
	size_t totalSize = size * nmemb;
	output->append(static_cast<char *>(contents), totalSize);
	return totalSize;
}

json curl_post(const std::string &data, const std::string &url, const std::vector<std::string> &header = {}) {
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
		// If the user passed in headers, iterate through and assign them to the linked list
		if (!header.empty()) {
			for (const std::string &head: header) {
				headers = curl_slist_append(headers, head.c_str());
			}
		}
		// Set the callback function to handle the response
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
		// Perform the GET request
		response_code = curl_easy_perform(curl);
		if (response_code == CURLE_OK) {
			curl_easy_cleanup(curl);
			curl_slist_free_all(headers);
			curl_global_cleanup();
      std::cout << response_data << std::endl;
			return json::parse(response_data);
		} else {
			std::cerr << "cURL request failed: " << curl_easy_strerror(response_code) << std::endl;
			curl_easy_cleanup(curl);
			curl_slist_free_all(headers);
			curl_global_cleanup();
			return json::parse(fmt::format("error: {}", curl_easy_strerror(response_code)));
		}
	}
	// Clean up libcurl
	curl_global_cleanup();
	return json::parse(fmt::format("error: unknown error occurred in making API request!"));
}

std::string curl_request(const std::string &post_data, const std::string &url, const std::vector<std::string> &header = {}) {
	CURL *curl;
	CURLcode response_code;
	std::string response;
	curl_global_init(CURL_GLOBAL_DEFAULT);
	curl = curl_easy_init();
	if (curl) {
		struct curl_slist *headers = nullptr;
		// Append the headers to the curl linked list
		headers = curl_slist_append(headers, "Content-Type: application/json");
		// If the user passed in headers, iterate through and assign them to the linked list
		if (!header.empty()) {
			for (const std::string &head: header) {
				headers = curl_slist_append(headers, head.c_str());
			}
		}
		// Set options
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		// Set the response data variable as the cURL write callback data
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
		// Perform the HTTP request
		response_code = curl_easy_perform(curl);
		// Check if the response was OK
		if (response_code == CURLE_OK) {
			curl_easy_cleanup(curl);
			curl_slist_free_all(headers);
			curl_global_cleanup();
			return response;
		} else {
			std::cerr << "cURL request failed: " << curl_easy_strerror(response_code) << std::endl;
		}
	}
	return R"({"error": "Request Failed!"})";
}

void replace_raw_char(std::string &str, const char refchar) {
    size_t found = str.find(refchar);
    while (found != std::string::npos) {
        std::string replace = fmt::format(R"(\{})", refchar);
        str.replace(found, 1, replace);
        found = str.find('\"', found + 2);
    }
}

std::string get_ollama_chat(const std::string &prompt) {
	std::string endpoint = get_env("LOCAL_ENDPOINT") + "/api/v1/chat";
  std::string formatted_prompt = prompt;
  replace_raw_char(formatted_prompt, '"');
	auto res = curl_post(fmt::format(R"({{ "chat": "{}" }})", formatted_prompt), endpoint);
	return res.contains("response") ? res["response"] : res["error"];
}

std::string get_ollama_code(const std::string &prompt) {
  std::string endpoint = get_env("LOCAL_ENDPOINT") + "/api/v1/code";
  std::string formatted_prompt = prompt;
  replace_raw_char(formatted_prompt, '"');
  auto response = curl_post(fmt::format(R"({{ "chat": "{}" }})", formatted_prompt), endpoint);
  
  return response.contains("response") ? response["response"] : response["error"];
}

std::string get_ollama_klim(const std::string &prompt) {
  std::string endpoint = get_env("LOCAL_ENDPOINT") + "/api/v1/klim";
  std::string formatted_prompt = prompt;
  replace_raw_char(formatted_prompt, '"');
  auto response = curl_post(fmt::format(R"({{ "chat": "{}" }})", formatted_prompt), endpoint);
  return response.contains("response") ? response["response"] : response["error"];
}

std::string get_ollama_ethan(const std::string &prompt) {
  std::string endpoint = get_env("LOCAL_ENDPOINT") + "/api/v1/ethan";
  std::string formatted_prompt = prompt;
  replace_raw_char(formatted_prompt, '"');
  auto response = curl_post(fmt::format(R"({{ "chat": "{}" }})", formatted_prompt), endpoint);
  return response.contains("response") ? response["response"] : response["error"];
}

json post(const std::string &request, const std::string &endpoint) {
	std::string url = get_env("LOCAL_ENDPOINT") + endpoint;
	return curl_post(request, url);
}

std::string get_ollama_describe(const std::string &url) {
	std::string endpoint = get_env("LOCAL_ENDPOINT");
	endpoint = endpoint + "/api/v1/describe";

  std::cout << url << std::endl;
	
  auto res = curl_post(fmt::format(R"({{ "url": "{}"}})", url), endpoint);
	return res.contains("response") ? res["response"] : res["error"];
}

std::string get_image(const std::string &prompt) {
	std::string endpoint = get_env("LOCAL_ENDPOINT");
	endpoint = endpoint + "/api/v1/generate";
	auto res = curl_post(fmt::format(R"({{ "prompt": "{}"}})", prompt), endpoint);
	return res["response"];
}

std::string get_tts_response(const std::string &prompt) {
	return curl_request(prompt, "https://play.ht/api/v2/tts/stream");
}

std::string get_reddit_response(const std::string &subreddit, const std::string &filter) {
	std::string URL = fmt::format("https://old.reddit.com/r/{}/top/.json?sort=top&limit=1&t={}", subreddit, filter);
	// Get the command to extract the URL
	std::string command = fmt::format("curl -X GET -L \"{}\"", URL);
	char buffer[128];
	std::string response;
	// Convert to a C-string
	const char *cmd = command.c_str();
	// Open the pipe to process to command
	FILE *pipe = popen(cmd, "r");
	// Check that the pipe was opened successfully
	if (!pipe) {
		std::cerr << "Error opening pipe" << std::endl;
	}
	// Write contents of stdout buf to c++ style string
	while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
		response += buffer;
	}

	return response;
}

constexpr bool is_whitespace(char c) {
	return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

constexpr std::string_view trim_whitespace(std::string_view str) {
	size_t start = 0;
	size_t end = str.length();

	while (start < end && is_whitespace(str[start])) {
		++start;
	}

	while (end > start && is_whitespace(str[end - 1])) {
		--end;
	}

	return str.substr(start, end - start);
}

constexpr std::string_view get_key_value(const std::string_view& line, bool is_key) {
	size_t equalPos = line.find('=');
	return trim_whitespace(is_key ? line.substr(0, equalPos) : line.substr(equalPos + 1));
}
//
//constexpr std::string_view get_env(const std::string_view& given_key, const std::string_view& lines) {
//	size_t startPos = 0;
//	size_t endPos = lines.find('\n');
//
//	while (endPos != std::string_view::npos) {
//		const std::string_view line = lines.substr(startPos, endPos - startPos);
//
//		if (line.find('=') != std::string::npos && line.find('=') > 0) {
//			const std::string_view key = get_key_value(line, true);
//			const std::string_view value = get_key_value(line, false);
//
//			if (key == given_key) {
//				return value;
//			}
//		}
//
//		startPos = endPos + 1;
//		endPos = lines.find('\n', startPos);
//	}
//
//	return "";
//}
//
inline std::string get_env(const std::string_view &given_key) {
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

		// Check if the line contains an equal sign and it is not the first character
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
