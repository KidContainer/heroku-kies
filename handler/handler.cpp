#include "handler.hpp"
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

#include <memory>

#include "../utils/file.hpp"

using namespace cinatra;

namespace handler
{

    void upload_file(cinatra::request &req, cinatra::response &res)
    {
        if(req.head()==nullptr){
            SPDLOG_INFO("head is empty");
        }
        if(req.body()==nullptr){
            SPDLOG_INFO("body is empty");

        }
        auto file = req.get_file();
        if(file==nullptr){
            res.set_status_and_content(status_type::bad_request,"no file provided", req_content_type::string);
            return;
        }
        SPDLOG_INFO("size={}, file={}",file->get_file_size(), file->get_file_path());
        res.set_status_and_content(status_type::ok,file->get_file_path(),req_content_type::multipart);

    }

} // namespace handler
