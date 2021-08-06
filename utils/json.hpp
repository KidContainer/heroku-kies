#ifndef UTILS_JSON_HPP
#define UTILS_JSON_HPP

#include <string>
#include <string_view>
#include <nlohmann/json.hpp>
namespace utils
{
    std::string resp(int status_code=0, std::string_view status_message="", nlohmann::json data=nlohmann::json());

    bool all_string(const nlohmann::json& data, std::initializer_list<std::string> names);

    bool all_number(const nlohmann::json& data, std::initializer_list<std::string> names);

    bool all_ingeter(const nlohmann::json& data, std::initializer_list<std::string> names);

    bool all_float(const nlohmann::json& data, std::initializer_list<std::string> names);

    bool all_boolean(const nlohmann::json& data, std::initializer_list<std::string> names);


} // namespace utils


#endif