#include <fmt/format.h>
#include "utils/http/cringe_curl.h"

CringeCurl::CringeCurl() {
	curl_global_init(CURL_GLOBAL_DEFAULT);
	curl = curl_easy_init();
}

CringeCurl::~CringeCurl() {
	curl_global_cleanup();
	curl_easy_cleanup(curl);
}

size_t CringeCurl::writeCallback(void *contents, size_t size, size_t nmemb, std::string *output) {
	size_t totalSize = size * nmemb;
	output->append((char*)contents, totalSize);
	return totalSize;
}

json CringeCurl::post(const std::string &data, const std::string &url, const std::vector<std::string> &headers) {
	json response;
	std::string response_data;
	struct curl_slist *request_headers = nullptr;
	request_headers = curl_slist_append(request_headers, "Content-Type: application/json");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, request_headers);
	for (const std::string &head : headers) {
		request_headers = curl_slist_append(request_headers, head.c_str());
	}
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, this->writeCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
	curl_slist_free_all(request_headers);
	return json::parse(response_data);
}

json CringeCurl::get(const std::string &url, const std::vector<std::string> &headers) {
	std::string response;
	struct curl_slist *request_headers = nullptr;
	for (const std::string &head : headers) {
		request_headers = curl_slist_append(request_headers, head.c_str());
	}
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, request_headers);
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
	curl_slist_free_all(request_headers);
	return json::parse(response);
};
