#include "helper.hpp"
#include <nlohmann/json.hpp>

namespace utils
{
    std::string resp(int status_code, std::string_view status_message, nlohmann::json data){
        nlohmann::json j;
        j["status_code"] = status_code;
        j["status_message"] = status_message;
        j["data"] = data;
        return j.dump(4);
    }


} // namespace utils
