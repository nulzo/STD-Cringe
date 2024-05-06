#ifndef CRINGE_CRINGE_CURL_H
#define CRINGE_CRINGE_CURL_H

#include <dpp/dpp.h>
#include <curl/curl.h>

using json = nlohmann::json;

class CringeCurl {
public:
	CringeCurl();
	~CringeCurl();
	json post(const std::string &data, const std::string &url, const std::vector<std::string> &headers = {});
	json get(const std::string &url, const std::vector<std::string> &headers = {});
private:
	CURL* curl;
	static size_t writeCallback(void *contents, size_t size, size_t nmemb, std::string *output);
};

#endif
