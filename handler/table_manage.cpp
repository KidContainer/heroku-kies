#include "table_manage.hpp"
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

#include "../db/db_operate.hpp"
#include "../db/t_user_info.hpp"
#include "../db/db_manager.hpp"

using namespace cinatra;

namespace handler
{
    void t_user_info_op(request &req, response &res){
        SPDLOG_INFO("operation for t_user_info, ip={}", req.get_header_value("X-Forwarded-For"));
        
        nlohmann::json resp;

        //content type check
        if(req.get_content_type() != content_type::string){
            SPDLOG_WARN("Unsupported ContentType, which is {}", req.get_content_type());
            resp["status_code"] = 10001;
            resp["status_message"] = "unsupported Content-Type";
            res.set_status_and_content(status_type::ok, resp.dump(), req_content_type::json);
            return;
        }

        //json valid check
        if(!nlohmann::json::accept(req.body())){
            SPDLOG_WARN("Request body is not a valid json, it's {}", req.body());
            resp["status_code"] = 10001;
            resp["status_message"] = "it's not a valid json";
            res.set_status_and_content(status_type::ok, resp.dump(), req_content_type::json);
            return;
        }

        nlohmann::json request = nlohmann::json::parse(req.body());

        // get op type
        if(!request.contains("op_type")){
            SPDLOG_WARN("Request does not contains op_type");
            resp["status_code"] = 10001;
            resp["status_message"] = "op_type is needed";
            resp["op_type_enum"] = {"insert","remove","fetch","update","insert_multi"};
            res.set_status_and_content(status_type::ok, resp.dump(), req_content_type::json);
            return;
        }


        auto op_type = request.value("op_type","none");
        SPDLOG_INFO("op_type is {}", op_type);

        resp["status_code"] = 0;
        resp["status_message"] = "";
        res.set_status_and_content(status_type::ok, resp.dump(), req_content_type::json);
    }

} // namespace handler
