#include "handler.hpp"
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

#include <memory>

#include "../db/t_flow.hpp"
#include "../utils/file.hpp"

using namespace cinatra;

namespace handler
{

    void upload_file(cinatra::request &req, cinatra::response &res)
    {
        SPDLOG_INFO("request={}, filename={}", req.data(), req.get_filename_from_path());
        auto file = req.get_file();
        auto buf = std::make_unique<char[]>(file->get_file_size()+1);
        file->write(buf.get(), file->get_file_size());
        res.set_status_and_content(status_type::ok,std::string{buf.get(),file->get_file_size()},req_content_type::string, content_encoding::gzip);
    }

} // namespace handler
