#ifndef CRINGE_CRINGE_64_H
#define CRINGE_CRINGE_64_H

#include <string>
#include <string_view>

auto cringe64_encode (std::string const& string, bool url = false) -> std::string;
auto cringe64_decode(std::string const& string, bool remove_linebreaks = false) -> std::string;
auto cringe64_encode(unsigned char const*, size_t len, bool url = false) -> std::string;
#if __cplusplus >= 201703L
auto cringe64_encode (std::string_view string, bool url = false) -> std::string;
#endif

#endif
