#include "json.hpp"
#include <nlohmann/json.hpp>

#include <iostream>

namespace utils
{
    std::string resp(int status_code, std::string_view status_message, nlohmann::json data)
    {
        nlohmann::json j;
        j["status_code"] = status_code;
        j["status_message"] = status_message;
        j["data"] = data;
        return j.dump(4);
    }

    template <nlohmann::json::value_t... type>
    inline bool all_type(const nlohmann::json &data, std::initializer_list<std::string> names)
    {
        return std::all_of(names.begin(), names.end(), [&data](const std::string &name)
                           { return data.contains(name) && ((data[name].type() == type) || ...); });
    }

    bool all_string(const nlohmann::json &data, std::initializer_list<std::string> names)
    {
        return all_type<nlohmann::json::value_t::string>(data, names);
    }

    bool all_number(const nlohmann::json &data, std::initializer_list<std::string> names)
    {
        return all_type<nlohmann::json::value_t::number_float, nlohmann::json::value_t::number_integer, nlohmann::json::value_t::number_unsigned>(data, names);
    }

    bool all_ingeter(const nlohmann::json &data, std::initializer_list<std::string> names)
    {
        return all_type<nlohmann::json::value_t::number_integer, nlohmann::json::value_t::number_unsigned>(data, names);
    }

    bool all_float(const nlohmann::json &data, std::initializer_list<std::string> names)
    {
        return all_type<nlohmann::json::value_t::number_float>(data, names);
    }

    bool all_boolean(const nlohmann::json &data, std::initializer_list<std::string> names)
    {
        return all_type<nlohmann::json::value_t::boolean>(data, names);
    }

} // namespace utils
