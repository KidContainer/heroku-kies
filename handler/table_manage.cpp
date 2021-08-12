#include "table_manage.hpp"
#include <spdlog/spdlog.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <nlohmann/json.hpp>

#include "../db/db_operate.hpp"
#include "../db/t_user_info.hpp"
#include "../db/db_manager.hpp"
#include "../db/t_blog.hpp"

#include "../utils/json.hpp"
#include "../utils/time.hpp"
#include "../utils/log.hpp"

using namespace cinatra;

namespace handler
{
    void t_user_info_op(request &req, response &res)
    {
        auto log_id = utils::req_id(req);
        SPDLOG_INFO("log_id={}, operation for t_user_info, ip={}",log_id, req.get_header_value("X-Forwarded-For"));

        //content type check
        if (req.get_content_type() != content_type::string)
        {
            SPDLOG_WARN("log_id={}, Unsupported ContentType, which is {}", log_id, req.get_content_type());
            res.set_status_and_content(status_type::ok, utils::resp(10001, "unsupported Content-Type"), req_content_type::json);
            return;
        }

        //json valid check
        if (!nlohmann::json::accept(req.body()))
        {
            SPDLOG_WARN("log_id={}, Request body is not a valid json, it's {}",log_id, req.body());
            res.set_status_and_content(status_type::ok, utils::resp(10001, "it's not a valid json"), req_content_type::json);
            return;
        }

        nlohmann::json request = nlohmann::json::parse(req.body());

        // get op type
        if (!utils::all_string(request, {"op_type"}))
        {
            SPDLOG_WARN("log_id={}, Request does not contains op_type", log_id);
            nlohmann::json data;
            data["op_type_enum"] = {"insert", "remove", "fetch", "update", "multi_insert", "multi_fetch"};
            res.set_status_and_content(status_type::ok, utils::resp(10001, "op_type is needed", data), req_content_type::json);
            return;
        }

        const auto op_type = request["op_type"].get<std::string>();
        if (op_type == "insert")
        {
            //Get the parameter
            if (!utils::all_string(request, {"user_name", "password"}))
            {
                SPDLOG_INFO("log_id={}, user_name or password is not string", log_id);
                res.set_status_and_content(status_type::ok, utils::resp(10001, "user_name or password is not string"), req_content_type::json);
                return;
            }
            if (!utils::all_ingeter(request, {"gender"}))
            {
                SPDLOG_INFO("log_id={}, gender is not integer", log_id);
                res.set_status_and_content(status_type::ok, utils::resp(10001, "gender is not integer"), req_content_type::json);
                return;
            }

            //Check if the name has been occupied
            bool exist = false;
            std::tie(std::ignore, exist) = db::t_user_info::fetch_first(log_id, {{"user_name", request["user_name"].get<std::string>()}});
            if (exist)
            {
                SPDLOG_INFO("log_id={}, user name '{}' has been registered", log_id, request["user_name"].get<std::string>());
                res.set_status_and_content(status_type::ok, utils::resp(10001, "user name has been occupied"), req_content_type::json);
                return;
            }

            //Insert
            std::unordered_map<std::string_view, std::any> data = {{"user_name", request["user_name"].get<std::string>()},
                                                                   {"password", request["password"].get<std::string>()},
                                                                   {"gender", request["gender"].get<std::int8_t>()},
                                                                   {"create_time", utils::now()}};
            if (auto profile = utils::get_string(request, "profile", ""); profile != "")
            {
                data.insert({"profile", profile});
            }
            if (auto email = utils::get_string(request, "email", ""); email != "")
            {
                data.insert({"email", email});
            }

            auto result = db::t_user_info::insert(log_id, data);
            if (result.affected_rows() == 0)
            {
                SPDLOG_INFO("log_id={}, insert failed", log_id);
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
                SPDLOG_INFO("log_id={}, user_name is missing", log_id);
                res.set_status_and_content(status_type::ok, utils::resp(10001, "user_name is missing"), req_content_type::json);
                return;
            }

            //Remove the user_name
            auto result = db::t_user_info::remove(log_id, {{"user_name", user_name}});
            if (result.affected_rows() == 0)
            {
                SPDLOG_INFO("log_id={}, failed to remove", log_id);
                res.set_status_and_content(status_type::ok, utils::resp(10001, "failed to remove"), req_content_type::json);
                return;
            }

            res.set_status_and_content(status_type::ok, utils::resp(), req_content_type::json);
            return;
        }
        else if (op_type == "fetch")
        {
            //Get the parameter
            auto condition = utils::retrieve_if_exist(request, {"user_name", "create_time", "last_login", "email", "profile", "gender"});

            //Get the result
            auto [user_info, exist] = db::t_user_info::fetch_first(log_id, condition);
            if (!exist)
            {
                SPDLOG_INFO("log_id={}, data does not exist, condition={}",log_id, request.dump());
                res.set_status_and_content(status_type::ok, utils::resp(10001, fmt::format("data does not exist, condition={}", request.dump())), req_content_type::json);
                return;
            }
            nlohmann::json result;
            result["user_name"] = user_info.user_name;
            result["gender"] = user_info.gender;
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
                SPDLOG_INFO("log_id={}, user_name is missing", log_id);
                res.set_status_and_content(status_type::ok, utils::resp(10001, "user_name is missing"), req_content_type::json);
                return;
            }

            //Check exist
            bool exist;
            std::tie(std::ignore, exist) = db::t_user_info::fetch_first(log_id,{{"user_name", user_name}});
            if (!exist)
            {
                SPDLOG_INFO("log_id={}, user {} does not exist", log_id, user_name);
                res.set_status_and_content(status_type::ok, utils::resp(10001, fmt::format("user {} does not exist", user_name)), req_content_type::json);
                return;
            }

            //Update
            auto data = utils::retrieve_if_exist(request, {"email", "profile", "last_login", "password", "gender"});

            auto result = db::t_user_info::update(log_id, {{"user_name", user_name}}, data);
            if (result.affected_rows() == 0)
            {
                SPDLOG_INFO("log_id={}, failed to update", log_id);
                res.set_status_and_content(status_type::ok, utils::resp(10001, "failed to update"), req_content_type::json);
                return;
            }
            res.set_status_and_content(status_type::ok, utils::resp(), req_content_type::json);
            return;
        }
        else if (op_type == "multi_insert")
        {
            if (!request.contains("users") || !request["users"].is_array())
            {
                SPDLOG_INFO("log_id={}, please use array named users to upload data", log_id);
                res.set_status_and_content(status_type::ok, utils::resp(10001, "please use array named users to upload data"), req_content_type::json);
                return;
            }
            auto users = request["users"];

            for (const auto &item : users)
            {
                //Get the parameter
                if (!utils::all_string(item, {"user_name", "password"}))
                {
                    SPDLOG_INFO("log_id={}, user_name or password is not string", log_id);
                    res.set_status_and_content(status_type::ok, utils::resp(10001, "user_name or password is not string"), req_content_type::json);
                    return;
                }

                if (!utils::all_ingeter(item, {"gender"}))
                {
                    SPDLOG_INFO("log_id={}, gender is not integer", log_id);
                    res.set_status_and_content(status_type::ok, utils::resp(10001, "gender is not integer"), req_content_type::json);
                    return;
                }
            }

            // Check if contains the same data
            std::unordered_set<std::string> s;
            s.reserve(users.size());
            for (auto &&item : users)
            {
                if (s.find(std::string(item["user_name"])) != s.end())
                {
                    SPDLOG_INFO("log_id={}, there are duplicated data", log_id);
                    res.set_status_and_content(status_type::ok, utils::resp(10001, "there are duplicated data"), req_content_type::json);
                    return;
                }
                s.insert(std::string(item["user_name"]));
            }

            //Check if database has the key
            for (const auto &item : users)
            {
                //Check if the name has been occupied
                bool exist = false;
                std::tie(std::ignore, exist) = db::t_user_info::fetch_first(log_id, {{"user_name", item["user_name"].get<std::string>()}});
                if (exist)
                {
                    SPDLOG_INFO("log_id={}, user name '{}' has been registered", log_id, item["user_name"].get<std::string>());
                    res.set_status_and_content(status_type::ok, utils::resp(10001, "user name has been occupied"), req_content_type::json);
                    return;
                }
            }

            std::vector<std::unordered_map<std::string_view, std::any>> all_data;
            for (const auto &item : users)
            {
                //Insert
                std::unordered_map<std::string_view, std::any> data = {{"user_name", item["user_name"].get<std::string>()},
                                                                       {"password", item["password"].get<std::string>()},
                                                                       {"gender", item["gender"].get<std::uint8_t>()},
                                                                       {"create_time", utils::now()}};
                if (auto profile = utils::get_string(item, "profile", ""); profile != "")
                {
                    data.insert({"profile", profile});
                }
                if (auto email = utils::get_string(item, "email", ""); email != "")
                {
                    data.insert({"email", email});
                }

                all_data.push_back(data);
            }
            auto result = db::t_user_info::insert(log_id, all_data);
            if (result.affected_rows() == 0)
            {
                SPDLOG_INFO("log_id={}, insert failed", log_id);
                res.set_status_and_content(status_type::ok, utils::resp(10001, "insert failed"), req_content_type::json);
                return;
            }
            res.set_status_and_content(status_type::ok, utils::resp(0, ""), req_content_type::json);
            return;
        }
        else if (op_type == "multi_fetch")
        {
            auto condition = utils::retrieve_if_exist(request, {"user_name", "create_time", "last_login", "email", "profile"});

            //Get the result
            auto users_info = db::t_user_info::fetch(log_id, condition);
            std::vector<nlohmann::json> results;
            results.reserve(users_info.size());
            for (auto &&item : users_info)
            {
                nlohmann::json result;
                result["user_name"] = item.user_name;
                result["gender"] = item.gender;
                result["create_time"] = item.create_time;
                result["last_login"] = item.last_login;
                result["email"] = item.email;
                result["profile"] = item.profile;

                results.push_back(result);
            }
            res.set_status_and_content(status_type::ok, utils::resp(0, "", results), req_content_type::json);
            return;
        }
        res.set_status_and_content(status_type::ok, utils::resp(10001, "op_type is not supported"), req_content_type::json);
    }

    void t_blog_op(cinatra::request &req, cinatra::response &res)
    {
        auto log_id = utils::req_id(req);
        SPDLOG_INFO("log_id={}, operation for t_blog_op, ip={}", log_id, req.get_header_value("X-Forwarded-For"));

        //content type check
        if (req.get_content_type() != content_type::string)
        {
            SPDLOG_WARN("log_id={}, Unsupported ContentType, which is {}", log_id, req.get_content_type());
            res.set_status_and_content(status_type::ok, utils::resp(10001, "unsupported Content-Type"), req_content_type::json);
            return;
        }

        //json valid check
        if (!nlohmann::json::accept(req.body()))
        {
            SPDLOG_WARN("log_id={}, Request body is not a valid json, it's {}", log_id, req.body());
            res.set_status_and_content(status_type::ok, utils::resp(10001, "it's not a valid json"), req_content_type::json);
            return;
        }

        nlohmann::json request = nlohmann::json::parse(req.body());

        // get op type
        if (!utils::all_string(request, {"op_type"}))
        {
            SPDLOG_WARN("log_id={}, Request does not contains op_type", log_id);
            nlohmann::json data;
            data["op_type_enum"] = {"insert", "remove", "fetch", "update", "multi_insert", "multi_fetch"};
            res.set_status_and_content(status_type::ok, utils::resp(10001, "op_type is needed", data), req_content_type::json);
            return;
        }

        const auto op_type = request["op_type"].get<std::string>();
        if (op_type == "insert")
        {
            //Get the parameter
            if (!utils::all_string(request, {"user_id", "content", "title", "sub_title", "tags", "images"}))
            {
                SPDLOG_INFO("log_id={}, parameter is not missing", log_id);
                res.set_status_and_content(status_type::ok, utils::resp(10001, "parameter is missing"), req_content_type::json);
                return;
            }

            auto data = utils::retrieve_if_exist(request, {"user_id", "content", "title", "sub_title", "tags", "images"});

            //Insert

            auto result = db::t_blog::insert(log_id, data);
            if (result.affected_rows() == 0)
            {
                SPDLOG_INFO("log_id={}, insert failed", log_id);
                res.set_status_and_content(status_type::ok, utils::resp(10001, "insert failed"), req_content_type::json);
                return;
            }

            res.set_status_and_content(status_type::ok, utils::resp(), req_content_type::json);
            return;
        }
        else if (op_type == "remove")
        {
            //Get the parameter
            std::int64_t id;
            if (id = utils::get_number(request, "id", -1); id == -1)
            {
                SPDLOG_INFO("log_id={}, id is missing", log_id);
                res.set_status_and_content(status_type::ok, utils::resp(10001, "id is missing"), req_content_type::json);
                return;
            }

            //Remove the user_name
            auto result = db::t_blog::remove(log_id,{{"id", id}});
            if (result.affected_rows() == 0)
            {
                SPDLOG_INFO("log_id={}, failed to remove", log_id);
                res.set_status_and_content(status_type::ok, utils::resp(10001, "failed to remove"), req_content_type::json);
                return;
            }

            res.set_status_and_content(status_type::ok, utils::resp(), req_content_type::json);
            return;
        }
        else if (op_type == "fetch")
        {
            //Get the parameter
            auto condition = utils::retrieve_if_exist(request, {"id", "user_id", "title"});

            //Get the result
            auto [blog, exist] = db::t_blog::fetch_first(log_id, condition);
            if (!exist)
            {
                SPDLOG_INFO("log_id={}, data does not exist, condition={}", log_id, request.dump());
                res.set_status_and_content(status_type::ok, utils::resp(10001, fmt::format("data does not exist, condition={}", request.dump())), req_content_type::json);
                return;
            }
            nlohmann::json result;
            result["id"] = blog.id;
            result["user_id"] = blog.user_id;
            result["content"] = blog.content;
            result["modified_count"] = blog.modified_count;
            result["create_time"] = blog.create_time;
            result["update_time"] = blog.update_time;
            result["title"] = blog.title;
            result["sub_title"] = blog.sub_title;
            result["tags"] = blog.tags;
            result["images"] = blog.images;

            res.set_status_and_content(status_type::ok, utils::resp(0, "", result), req_content_type::json);
            return;
        }
        else if (op_type == "update")
        {
            //Get the user name
            std::int64_t id;
            if (id = utils::get_number(request, "id", -1); id == -1)
            {
                SPDLOG_INFO("log_id={}, id is missing", log_id);
                res.set_status_and_content(status_type::ok, utils::resp(10001, "id is missing"), req_content_type::json);
                return;
            }

            //Check exist
            bool exist;
            std::tie(std::ignore, exist) = db::t_blog::fetch_first(log_id, {{"id", id}});
            if (!exist)
            {
                SPDLOG_INFO("log_id={}, id {} does not exist", log_id, id);
                res.set_status_and_content(status_type::ok, utils::resp(10001, fmt::format("id {} does not exist", id)), req_content_type::json);
                return;
            }

            //Update
            auto data = utils::retrieve_if_exist(request, {"content", "create_time", "update_time", "modified_count", "title", "sub_title", "tags", "images"});

            auto result = db::t_user_info::update(log_id, {{"id", id}}, data);
            if (result.affected_rows() == 0)
            {
                SPDLOG_INFO("log_id={}, failed to update", log_id);
                res.set_status_and_content(status_type::ok, utils::resp(10001, "failed to update"), req_content_type::json);
                return;
            }
            res.set_status_and_content(status_type::ok, utils::resp(), req_content_type::json);
            return;
        }
        else if (op_type == "multi_insert")
        {
            if (!request.contains("blogs") || !request["blogs"].is_array())
            {
                SPDLOG_INFO("log_id={}, please use array named blogs to upload data", log_id);
                res.set_status_and_content(status_type::ok, utils::resp(10001, "please use array named blogs to upload data"), req_content_type::json);
                return;
            }
            auto blogs = request["blogs"];

            for (const auto &item : blogs)
            {
                //Get the parameter
                if (!utils::all_string(item, {"user_id", "content", "title", "sub_title", "tags", "images"}))
                {
                    SPDLOG_INFO("log_id={}, parameter is wrong", log_id);
                    res.set_status_and_content(status_type::ok, utils::resp(10001, "parameter is wrong"), req_content_type::json);
                    return;
                }
            }

            std::vector<std::unordered_map<std::string_view, std::any>> all_data;
            for (const auto &item : blogs)
            {
                //Insert
                auto data = utils::retrieve_if_exist(item, {"user_id", "content", "title", "sub_title", "tags", "images"});

                all_data.push_back(data);
            }
            auto result = db::t_user_info::insert(log_id, all_data);
            if (result.affected_rows() == 0)
            {
                SPDLOG_INFO("log_id={}, insert failed", log_id);
                res.set_status_and_content(status_type::ok, utils::resp(10001, "insert failed"), req_content_type::json);
                return;
            }
            res.set_status_and_content(status_type::ok, utils::resp(0, ""), req_content_type::json);
            return;
        }
        else if (op_type == "multi_fetch")
        {
            auto condition = utils::retrieve_if_exist(request, {"id", "user_id", "title"});

            //Get the result
            auto blogs = db::t_blog::fetch(log_id, condition);
            std::vector<nlohmann::json> results;
            results.reserve(blogs.size());
            for (auto &&blog : blogs)
            {
                nlohmann::json result;
                result["id"] = blog.id;
                result["user_id"] = blog.user_id;
                result["content"] = blog.content;
                result["modified_count"] = blog.modified_count;
                result["create_time"] = blog.create_time;
                result["update_time"] = blog.update_time;
                result["title"] = blog.title;
                result["sub_title"] = blog.sub_title;
                result["tags"] = blog.tags;
                result["images"] = blog.images;

                results.push_back(result);
            }
            res.set_status_and_content(status_type::ok, utils::resp(0, "", results), req_content_type::json);
            return;
        }
        res.set_status_and_content(status_type::ok, utils::resp(10001, "op_type is not supported"), req_content_type::json);
    }

} // namespace handler
