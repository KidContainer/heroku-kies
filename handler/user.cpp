#include "user.hpp"
#include <spdlog/spdlog.h>


#include "../utils/log.hpp"
#include "../utils/json.hpp"

using namespace cinatra;

namespace handler
{
    void log_in(cinatra::request &req, cinatra::response &res)
    {
        auto log_id = utils::req_id(req);
        SPDLOG_INFO("log_id={}, log_in has been called", log_id);
        SPDLOG_DEBUG("log_id={}, content_type={}, true_content_type={}", req.get_content_type(), req.get_header_value("Content-Type"));
        //content type check
        if(req.get_content_type() != content_type::string){
            SPDLOG_ERROR("log_id={}, request type is not string, it's {}", req.get_content_type());
            res.set_status_and_content(status_type::ok, utils::resp(10001,"content type is wrong"), req_content_type::json);
            return;
        }

        res.set_status_and_content(status_type::ok, utils::resp(), req_content_type::json);

    }
    void log_out(cinatra::request &req, cinatra::response &res)
    {
        auto log_id = utils::req_id(req);
    }
    void remove_user(cinatra::request &req, cinatra::response &res)
    {
        auto log_id = utils::req_id(req);
    }
    void create_user(cinatra::request &req, cinatra::response &res)
    {
        auto log_id = utils::req_id(req);
    }
    void update_user_info(cinatra::request &req, cinatra::response &res)
    {
        auto log_id = utils::req_id(req);
    }
    void get_user_info(cinatra::request &req, cinatra::response &res)
    {
        auto log_id = utils::req_id(req);
    }
} // namespace handler
