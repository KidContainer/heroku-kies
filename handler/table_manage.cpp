#include "table_manage.hpp"
#include <spdlog/spdlog.h>
#include <string>
#include <nlohmann/json.hpp>

#include "../db/db_operate.hpp"
#include "../db/t_user_info.hpp"
#include "../db/db_manager.hpp"

#include "../utils/json.hpp"
#include "../utils/time.hpp"

using namespace cinatra;

namespace handler
{
    void t_user_info_op(request &req, response &res)
    {
        SPDLOG_INFO("operation for t_user_info, ip={}", req.get_header_value("X-Forwarded-For"));

        //content type check
        if (req.get_content_type() != content_type::string)
        {
            SPDLOG_WARN("Unsupported ContentType, which is {}", req.get_content_type());
            res.set_status_and_content(status_type::ok, utils::resp(10001, "unsupported Content-Type"), req_content_type::json);
            return;
        }

        //json valid check
        if (!nlohmann::json::accept(req.body()))
        {
            SPDLOG_WARN("Request body is not a valid json, it's {}", req.body());
            res.set_status_and_content(status_type::ok, utils::resp(10001, "it's not a valid json"), req_content_type::json);
            return;
        }

        nlohmann::json request = nlohmann::json::parse(req.body());

        // get op type
        if (!utils::all_string(request, {"op_type"}))
        {
            SPDLOG_WARN("Request does not contains op_type");
            nlohmann::json data;
            data["op_type_enum"] = {"insert", "remove", "fetch", "update", "insert_multi"};
            res.set_status_and_content(status_type::ok, utils::resp(10001, "op_type is needed", data), req_content_type::json);
            return;
        }

        const auto op_type = request["op_type"].get<std::string>();
        if (op_type == "insert")
        {
            //Get the parameter
            if (!utils::all_string(request, {"user_name", "password"}))
            {
                SPDLOG_INFO("user_name or password is not string");
                res.set_status_and_content(status_type::ok, utils::resp(10001, "user_name or password is not string"), req_content_type::json);
                return;
            }

            //Check if the name has been occupied
            bool exist = false;
            std::tie(std::ignore, exist) = db::t_user_info::fetch_first({{"user_name", request["user_name"].get<std::string>()}});
            if (exist)
            {
                SPDLOG_INFO("user name '{}' has been registered", request["user_name"].get<std::string>());
                res.set_status_and_content(status_type::ok, utils::resp(10001, "user name has been occupied"), req_content_type::json);
                return;
            }

            //Insert
            std::map<std::string_view, std::any> data = {{"user_name", request["user_name"].get<std::string>()},
                                                         {"password", request["password"].get<std::string>()},
                                                         {"create_time", utils::now()}};
            if (auto profile = utils::get_string(request, "profile", ""); profile != "")
            {
                data.insert({"profile", profile});
            }
            if (auto email = utils::get_string(request, "email", ""); email != "")
            {
                data.insert({"email", email});
            }
            auto result = db::t_user_info::insert(data);
            if (result.affected_rows() == 0)
            {
                SPDLOG_INFO("insert failed");
                res.set_status_and_content(status_type::ok, utils::resp(10001, "insert failed"), req_content_type::json);
                return;
            }

            res.set_status_and_content(status_type::ok, utils::resp(), req_content_type::json);
            return;
        }
        else if (op_type == "remove")
        {
            //Get the parameter
            std::string user_name = "";
            if (user_name = utils::get_string(request, "user_name", ""); user_name == "")
            {
                SPDLOG_INFO("user_name is missing");
                res.set_status_and_content(status_type::ok, utils::resp(10001, "user_name is missing"), req_content_type::json);
                return;
            }

            //Remove the user_name
            auto result = db::t_user_info::remove({{"user_name", user_name}});
            if (result.affected_rows() == 0)
            {
                SPDLOG_INFO("failed to remove");
                res.set_status_and_content(status_type::ok, utils::resp(10001, "failed to remove"), req_content_type::json);
                return;
            }

            res.set_status_and_content(status_type::ok, utils::resp(), req_content_type::json);
            return;
        }
        else if (op_type == "fetch")
        {
            //Get the parameter
            std::string user_name = "";
            if (user_name = utils::get_string(request, "user_name", ""); user_name == "")
            {
                SPDLOG_INFO("user_name is missing");
                res.set_status_and_content(status_type::ok, utils::resp(10001, "user_name is missing"), req_content_type::json);
                return;
            }

            //Get the result
            auto [user_info, exist] = db::t_user_info::fetch_first({{"user_name", user_name}});
            if (!exist)
            {
                SPDLOG_INFO("user {} does not exist", user_name);
                res.set_status_and_content(status_type::ok, utils::resp(10001, fmt::format("user {} does not exist", user_name)), req_content_type::json);
                return;
            }
            nlohmann::json result;
            result["user_name"] = user_info.user_name;
            result["create_time"] = user_info.create_time;
            result["last_login"] = user_info.last_login;
            result["email"] = user_info.email;
            result["profile"] = user_info.profile;

            res.set_status_and_content(status_type::ok, utils::resp(0, "", result), req_content_type::json);
            return;
        }
        else if (op_type == "update")
        {
            //Get the user name
            std::string user_name = "";
            if (user_name = utils::get_string(request, "user_name", ""); user_name == "")
            {
                SPDLOG_INFO("user_name is missing");
                res.set_status_and_content(status_type::ok, utils::resp(10001, "user_name is missing"), req_content_type::json);
                return;
            }

            //Check exist
            bool exist;
            std::tie(std::ignore, exist) = db::t_user_info::fetch_first({{"user_name", user_name}});
            if (!exist)
            {
                SPDLOG_INFO("user {} does not exist", user_name);
                res.set_status_and_content(status_type::ok, utils::resp(10001, fmt::format("user {} does not exist", user_name)), req_content_type::json);
                return;
            }

            //Update
            std::unordered_map<std::string_view, std::any> new_values;
            if (auto password = utils::get_string(request, "password", ""); password != "")
            {
                new_values.insert({"password", password});
            }
            if (auto email = utils::get_string(request, "email", ""); email != "")
            {
                new_values.insert({"email", email});
            }
            if (auto profile = utils::get_string(request, "profile", ""); profile != "")
            {
                new_values.insert({"profile", profile});
            }

            auto result = db::t_user_info::update({{"user_name", user_name}}, new_values);
            if (result.affected_rows() == 0)
            {
                SPDLOG_INFO("failed to update");
                res.set_status_and_content(status_type::ok, utils::resp(10001, "failed to update"), req_content_type::json);
                return;
            }
            res.set_status_and_content(status_type::ok, utils::resp(), req_content_type::json);
            return;
        }
        else if (op_type == "insert_multi")
        {
            if (!request.is_array())
            {
                SPDLOG_INFO("please use array to upload data");
                res.set_status_and_content(status_type::ok, utils::resp(10001, "please use array to upload data"), req_content_type::json);
                return;
            }
            std::map<std::string, std::string> result;
            for (const auto &item : request)
            {
                //Get the parameter
                if (!utils::all_string(item, {"user_name", "password"}))
                {
                    SPDLOG_INFO("user_name or password is not string");
                    res.set_status_and_content(status_type::ok, utils::resp(10001, "user_name or password is not string"), req_content_type::json);
                    continue;
                }

                //Check if the name has been occupied
                bool exist = false;
                std::tie(std::ignore, exist) = db::t_user_info::fetch_first({{"user_name", item["user_name"].get<std::string>()}});
                if (exist)
                {
                    SPDLOG_INFO("user name '{}' has been registered", item["user_name"].get<std::string>());
                    res.set_status_and_content(status_type::ok, utils::resp(10001, "user name has been occupied"), req_content_type::json);
                    return;
                }
            }

            std::map<std::string,std::string> all_result;
            for (const auto &item : request)
            {
                //Insert
                std::map<std::string_view, std::any> data = {{"user_name", item["user_name"].get<std::string>()},
                                                             {"password", item["password"].get<std::string>()},
                                                             {"create_time", utils::now()}};
                if (auto profile = utils::get_string(item, "profile", ""); profile != "")
                {
                    data.insert({"profile", profile});
                }
                if (auto email = utils::get_string(item, "email", ""); email != "")
                {
                    data.insert({"email", email});
                }
                auto result = db::t_user_info::insert(data);
                if (result.affected_rows() == 0)
                {
                    SPDLOG_INFO("insert failed");
                    auto user_name = item.get<std::string>();
                    all_result[user_name] = "insert failed";
                }
            }
            res.set_status_and_content(status_type::ok, utils::resp(0,"",all_result),req_content_type::json);
            return;
        }
        res.set_status_and_content(status_type::ok, utils::resp(10001, "op_type is not supported"), req_content_type::json);
    }

} // namespace handler
