#include "boost/date_time/posix_time/posix_time.hpp"


#include "handler.hpp"
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>
#include <inja/inja.hpp>

#include "../db/t_flow.hpp"

using namespace cinatra;


namespace handler{

    void home_page(cinatra::request& req, cinatra::response& res){
        auto ip_address = req.get_header_value("X-Forwarded-For");
        SPDLOG_INFO("ip={}",ip_address);
        nlohmann::json map;
        map["name"] = ip_address;
        map["current_time"] = boost::posix_time::to_iso_string(boost::posix_time::second_clock::local_time());
        inja::Environment env;
        
        res.set_status_and_content(status_type::ok,env.render_file("template/html/index.html", map), req_content_type::html, content_encoding::gzip);
    }


    void upload_file(cinatra::request& req, cinatra::response& res){
        auto ip_address = req.get_header_value("X-Forwarded-For");
        SPDLOG_INFO("ip={}",ip_address);
        SPDLOG_INFO("time={}", std::vector{1,2,3});
        res.set_status(status_type::ok);
    }

    
} // namespace handler


