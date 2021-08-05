#ifndef UTILS_HELPER_HPP
#define UTILS_HELPER_HPP

#include <string>
#include <string_view>
#include <nlohmann/json.hpp>
namespace utils
{
    std::string resp(int status_code=0, std::string_view status_message="", nlohmann::json data="{}"_json);
} // namespace utils


#endif