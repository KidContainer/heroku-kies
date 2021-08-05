#ifndef UTILS_HELPER_HPP
#define UTILS_HELPER_HPP

#include <string>
#include <string_view>

namespace utils
{
    std::string resp(int status_code, std::string_view status_message, std::string_view data="");
} // namespace utils


#endif