#ifndef CRINGE_CRINGE_REQUEST_H
#define CRINGE_CRINGE_REQUEST_H

#include <dpp/dpp.h>
#include <vector>
#include <string>

using json = nlohmann::json;
json cringe_post(const std::string &data, const std::string &url, const std::vector<std::string> &header = {});

#endif //CRINGE_CRINGE_REQUEST_H
