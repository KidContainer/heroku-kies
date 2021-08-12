#include "handler.hpp"
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

#include <memory>

#include "../utils/file.hpp"
#include "../utils/log.hpp"

using namespace cinatra;

namespace handler
{

    void upload_file(cinatra::request &req, cinatra::response &res)
    {
        auto log_id = utils::req_id(req);
        SPDLOG_INFO("log_id={}, upload file, ip={}", log_id, req.get_header_value("X-Forwarded-For"));
        auto file = req.get_file();
        if(file==nullptr){
            res.set_status_and_content(status_type::bad_request,"no file provided", req_content_type::string);
            return;
        }
        SPDLOG_INFO("log_id={}, size={}, file={}",log_id, file->get_file_size(), file->get_file_path());
        res.set_status_and_content(status_type::ok,file->get_file_path(),req_content_type::string);

    }

} // namespace handler
