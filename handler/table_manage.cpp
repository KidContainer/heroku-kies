#include "table_manage.hpp"
#include <spdlog/spdlog.h>
#include <string>
#include <nlohmann/json.hpp>

#include "../db/db_operate.hpp"
#include "../db/t_user_info.hpp"
#include "../db/db_manager.hpp"

#include "../utils/json.hpp"
#include "../utils/time.hpp"

using namespace cinatra;

namespace handler
{
    void t_user_info_op(request &req, response &res)
    {
        SPDLOG_INFO("operation for t_user_info, ip={}", req.get_header_value("X-Forwarded-For"));

        //content type check
        if (req.get_content_type() != content_type::string)
        {
            SPDLOG_WARN("Unsupported ContentType, which is {}", req.get_content_type());
            res.set_status_and_content(status_type::ok, utils::resp(10001, "unsupported Content-Type"), req_content_type::json);
            return;
        }

        //json valid check
        if (!nlohmann::json::accept(req.body()))
        {
            SPDLOG_WARN("Request body is not a valid json, it's {}", req.body());
            res.set_status_and_content(status_type::ok, utils::resp(10001, "it's not a valid json"), req_content_type::json);
            return;
        }

        nlohmann::json request = nlohmann::json::parse(req.body());

        // get op type
        if (!utils::all_string(request, {"op_type"}))
        {
            SPDLOG_WARN("Request does not contains op_type");
            nlohmann::json data;
            data["op_type_enum"] = {"insert", "remove", "fetch", "update", "insert_multi"};
            res.set_status_and_content(status_type::ok, utils::resp(10001, "op_type is needed", data), req_content_type::json);
            return;
        }

        auto op_type = request.value("op_type","none");
        if(op_type == "insert"){
            //Get the parameter
            if(!utils::all_string(request,{"user_name","password"})){
                SPDLOG_INFO("user_name or password is not string");
                res.set_status_and_content(status_type::ok, utils::resp(10001, "user_name or password is not string"), req_content_type::json);
                return;
            }

            //Check if the name has been occupied
            bool exist = false;
            std::tie(std::ignore, exist) = db::t_user_info::fetch_first({{"user_name",request["user_name"].get<std::string>()}});
            if(exist){
                SPDLOG_INFO("user name '{}' has been registered", request["user_name"].get<std::string>());
                res.set_status_and_content(status_type::ok, utils::resp(10001, "user name has been occupied"), req_content_type::json);
                return;
            }

            //Insert
            auto result = db::t_user_info::insert({
                {"user_name",request["user_name"].get<std::string>()},
                {"password", request["password"].get<std::string>()},
                {"create_time", utils::now()}});
            if(result.affected_rows()==1){
                SPDLOG_INFO("insert success");
                res.set_status_and_content(status_type::ok, utils::resp(), req_content_type::json);
                return;
            }
        }else if(op_type == "remove"){
            //Get the parameter
            if(utils::all_string(request,{"user_name"})){
                SPDLOG_INFO("user_name is missing");
                res.set_status_and_content(status_type::ok, utils::resp(10001,"user_name is missing"), req_content_type::json);
                return;
            }
            auto user_name = request[]

            //Remove the user_name
            auto result = db::t_user_info::remove({"user_name",})

        }
        res.set_status_and_content(status_type::ok, utils::resp(), req_content_type::json);
    }

} // namespace handler
