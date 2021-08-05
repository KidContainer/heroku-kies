#include "page.hpp"

// #include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

#include "../utils/file.hpp"

using namespace cinatra;


namespace handler
{
    void home_page(cinatra::request &req, cinatra::response &res)
    {
        // SPDLOG_INFO("homepage, ip={}", req.get_header_value("X-Forwarded-For"));

        res.set_status_and_content(status_type::ok, utils::read_template("template/html/index.html"), req_content_type::html, content_encoding::gzip);
    }


    void not_found_page(cinatra::request &req, cinatra::response &res){
        // SPDLOG_INFO("404 not found page, ip={}", req.get_header_value("X-Forwarded-For"));

        res.set_status_and_content(status_type::not_found, utils::read_template("template/html/404_not_found.html"), req_content_type::html, content_encoding::gzip);
    }


} // namespace handler
