#include "handler.hpp"
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

#include <memory>

#include "../utils/file.hpp"
#include "../db/db_operate.hpp"
#include "../db/t_user_info.hpp"
#include "../db/db_manager.hpp"

using namespace cinatra;

namespace handler
{

    void upload_file(cinatra::request &req, cinatra::response &res)
    {
        auto file = req.get_file();
        if(file==nullptr){
            res.set_status_and_content(status_type::bad_request,"no file provided", req_content_type::string);
            return;
        }
        SPDLOG_INFO("size={}, file={}",file->get_file_size(), file->get_file_path());
        res.set_status_and_content(status_type::ok,file->get_file_path(),req_content_type::multipart);

    }


    void debug_func(cinatra::request &req, cinatra::response &res){
        if(req.get_content_type() != content_type::multipart){
            SPDLOG_INFO("multipart");
        }else{
            SPDLOG_INFO("head={}, body={}", req.head(), req.body());
        }
        auto user_name = req.get_query_value("user_name");
        auto password = req.get_query_value("password");
        auto create_time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        auto result = db::t_user_info::insert({{"user_name",user_name},{"password",password},{"create_time",create_time}});
        db::Database::print_result(result);
        res.set_status_and_content(status_type::ok,"everything is greet", req_content_type::string);
    }


} // namespace handler
