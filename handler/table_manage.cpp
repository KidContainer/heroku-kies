#include "table_manage.hpp"
// #include <spdlog/spdlog.h>

#include "../db/db_operate.hpp"
#include "../db/t_user_info.hpp"
#include "../db/db_manager.hpp"

namespace handler
{
    void t_user_info_op(cinatra::request &req, cinatra::response &res){
        // SPDLOG_INFO("operation for t_user_info, ip={}", req.get_header_value("X-Forwarded-For"));
        auto user_name = req.get_query_value("user_name");
        auto password = req.get_query_value("password");
        if(user_name.empty() || password.empty()){
            // SPDLOG_INFO("parameters are missing");
            res.set_status_and_content(cinatra::status_type::ok,"parameters are missing");
        }
        auto result = db::t_user_info::insert({{"user_name",user_name},{"password",password}});
        // SPDLOG_INFO("result={}",db::Database::print_result(result));
        res.set_status_and_content(cinatra::status_type::ok, "inserting succeed");
    }

} // namespace handler
