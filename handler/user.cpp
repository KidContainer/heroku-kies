#include "user.hpp"
#include <spdlog/spdlog.h>

#include "../utils/log.hpp"
#include "../utils/json.hpp"
#include "../utils/cinatra.hpp"
#include "../dto/user.hpp"

using namespace cinatra;

namespace handler
{
    void log_in(cinatra::request &req, cinatra::response &res)
    {

        auto [param, log_id, success] = utils::parse_request<dto::UserLogInRequest>(req);
        if (!success)
        {
            SPDLOG_ERROR("log_id={}, failed to parse the parameters", log_id);
            res.set_status_and_content(status_type::ok, utils::resp(10000, "failed to parse the parameters"), req_content_type::json);
            return;
        }

        res.set_status_and_content(status_type::ok, utils::resp(), req_content_type::json);
    }

    void log_out(cinatra::request &req, cinatra::response &res)
    {
        auto [param, log_id, success] = utils::parse_request<dto::UserLogoutRequest>(req);
        if (!success)
        {
            SPDLOG_ERROR("log_id={}, failed to parse the parameters", log_id);
            res.set_status_and_content(status_type::ok, utils::resp(10000, "failed to parse the parameters"), req_content_type::json);
            return;
        }

        res.set_status_and_content(status_type::ok, utils::resp(), req_content_type::json);
    }

    void remove_user(cinatra::request &req, cinatra::response &res)
    {
        auto [param, log_id, success] = utils::parse_request<dto::RemoveUserRequest>(req);
        if (!success)
        {
            SPDLOG_ERROR("log_id={}, failed to parse the parameters", log_id);
            res.set_status_and_content(status_type::ok, utils::resp(10000, "failed to parse the parameters"), req_content_type::json);
            return;
        }

        res.set_status_and_content(status_type::ok, utils::resp(), req_content_type::json);
    }

    void create_user(cinatra::request &req, cinatra::response &res)
    {
        auto [param, log_id, success] = utils::parse_request<dto::CreateUserRequest>(req);
        if (!success)
        {
            SPDLOG_ERROR("log_id={}, failed to parse the parameters", log_id);
            res.set_status_and_content(status_type::ok, utils::resp(10000, "failed to parse the parameters"), req_content_type::json);
            return;
        }

        res.set_status_and_content(status_type::ok, utils::resp(), req_content_type::json);
    }

    void update_user_info(cinatra::request &req, cinatra::response &res)
    {
        auto [param, log_id, success] = utils::parse_request<dto::UpdateUserRequest>(req);
        if (!success)
        {
            SPDLOG_ERROR("log_id={}, failed to parse the parameters", log_id);
            res.set_status_and_content(status_type::ok, utils::resp(10000, "failed to parse the parameters"), req_content_type::json);
            return;
        }

        res.set_status_and_content(status_type::ok, utils::resp(), req_content_type::json);
    }

    void get_user_info(cinatra::request &req, cinatra::response &res)
    {
        auto [param, log_id, success] = utils::parse_request<dto::GetUserInfoRequest>(req);
        if (!success)
        {
            SPDLOG_ERROR("log_id={}, failed to parse the parameters", log_id);
            res.set_status_and_content(status_type::ok, utils::resp(10000, "failed to parse the parameters"), req_content_type::json);
            return;
        }
        dto::GetUserInfoResponse response;
        response.true_name = "Kie";
        res.set_status_and_content(status_type::ok, utils::resp(0, "", response), req_content_type::json);
    }
} // namespace handler
