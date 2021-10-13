#ifndef UTILS_CINATRA_HPP
#define UTILS_CINATRA_HPP

#include <cinatra.hpp>
#include <tuple>

namespace utils
{
    template <typename T>
    std::tuple<T, std::string_view, bool> parse_request(const cinatra::request &req)
    {
        auto log_id = utils::req_id(req);
        SPDLOG_INFO("log_id={}, content_type={}, true_content_type={}", log_id, req.get_content_type(), req.get_header_value("Content-Type"));
        //content type check
        if (req.get_content_type() != cinatra::content_type::string)
        {
            SPDLOG_ERROR("log_id={}, request type is not string, it's {}", log_id, req.get_content_type());
            return {T{}, log_id, false};
        }

        try
        {

            auto j = nlohmann::json::parse(req.body(), nullptr, false);
            if (j.is_discarded())
            {
                SPDLOG_ERROR("log_id={}, message=fail to parse json, body={}", req.body());
                return {T{}, log_id, false};
            }

            return {T{j}, log_id, true};
        }
        catch (std::exception &e)
        {
            SPDLOG_ERROR("log_id={}, request.body()={}, err={}", log_id,req.body(), e.what());
            return {T{}, log_id, false};
        }
    }
} // namespace utils

#endif