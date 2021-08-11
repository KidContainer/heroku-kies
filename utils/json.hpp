#ifndef UTILS_JSON_HPP
#define UTILS_JSON_HPP

#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <any>
#include <nlohmann/json.hpp>
namespace utils
{
    std::string resp(int status_code = 0, std::string_view status_message = "", nlohmann::json data = nlohmann::json());

    bool all_string(const nlohmann::json &data, std::initializer_list<std::string> names);

    bool all_number(const nlohmann::json &data, std::initializer_list<std::string> names);

    bool all_ingeter(const nlohmann::json &data, std::initializer_list<std::string> names);

    bool all_float(const nlohmann::json &data, std::initializer_list<std::string> names);

    bool all_boolean(const nlohmann::json &data, std::initializer_list<std::string> names);

    std::string get_string(const nlohmann::json &data, std::string name, std::string default_value);

    long long get_number(const nlohmann::json &data, std::string name, long long default_value);

    int get_integer(const nlohmann::json &data, std::string name, int default_value);

    double get_float(const nlohmann::json &data, std::string name, double default_value);

    bool all_boolean(const nlohmann::json &data, std::string name, bool default_value);

    std::vector<std::string> to_string_vector(const std::string_view data);

    std::unordered_map<std::string_view, std::any> retrieve_if_exist(const nlohmann::json& data, std::initializer_list<std::string_view> names);
} // namespace utils

#endif