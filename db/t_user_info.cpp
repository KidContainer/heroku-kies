#include "t_user_info.hpp"
#include "db_manager.hpp"
#include <spdlog/spdlog.h>
#include "../utils/any.hpp"

#include <vector>

namespace db
{
        std::string t_user_info::table_name()
        {
                return "t_user_info";
        }

        t_user_info t_user_info::row_to_user_info(pqxx::row res)
        {
                return t_user_info{
                    .id = res["id"].as<std::int64_t>(),
                    .user_name = res["user_name"].as<std::string>(),
                    .gender = res["gender"].as<std::int16_t>(),
                    .password = res["password"].as<std::string>(),
                    .create_time = res["create_time"].as<std::int64_t>(),
                    .last_login = res["last_login"].as<std::string>(),
                    .email = res["email"].as<std::string>(),
                    .profile = res["profile"].as<std::string>(),
                };
        }

        pqxx::result t_user_info::create_table()
        {
                pqxx::work t(Database::get_conn());
                auto sql = fmt::format(R"sql(CREATE TABLE IF NOT EXISTS {} (
                        id              SERIAL PRIMARY KEY,
                        user_name       VARCHAR(200) NOT NULL UNIQUE,
                        gender          SMALLINT NOT NULL,
                        password        VARCHAR(200) NOT NULL,
                        create_time     INTEGER NOT NULL,
                        last_login      TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
                        email           VARCHAR(100) DEFAULT '[no email]',
                        profile         VARCHAR(300) DEFAULT '/img/profile/default_profile.jpeg'
                ))sql",
                                       table_name());

                SPDLOG_INFO("sql={}", sql);
                auto result = t.exec(sql, "[CREATE TABLE]");
                t.commit();
                return result;
        }

        pqxx::result t_user_info::delete_table()
        {
                pqxx::work t(Database::get_conn());
                auto result = t.exec(fmt::format(R"sql(DROP TABLE {})sql", table_name()), "[DROP TABLE]");
                t.commit();
                return result;
        }

        std::vector<t_user_info> t_user_info::fetch(std::unordered_map<std::string_view, std::any> condition)
        {
                pqxx::work t(Database::get_conn());
                std::stringstream ss;
                ss << fmt::format(R"sql(SELECT * FROM {} )sql", table_name());
                if (condition.size() != 0)
                {
                        std::string sep = " ";
                        ss << R"sql(WHERE)sql";
                        for (auto &&item : condition)
                        {
                                ss << sep << item.first << "=" << t.quote(utils::any_to_string(item.second));
                                sep = " AND ";
                        }
                }
                SPDLOG_INFO("sql={}", ss.str());
                auto result = t.exec(ss.str(), "[FETCH]");
                t.commit();

                std::vector<t_user_info> user;
                user.reserve(result.size() + 1);
                for (auto row : result)
                {
                        user.push_back(row_to_user_info(row));
                }
                return user;
        }

        std::tuple<t_user_info, bool> t_user_info::fetch_first(std::unordered_map<std::string_view, std::any> condition)
        {
                pqxx::work t(Database::get_conn());
                std::stringstream ss;
                ss << fmt::format(R"sql(SELECT * FROM {})sql", table_name());
                if (!condition.empty())
                {
                        std::string sep = " ";
                        ss << R"sql( WHERE)sql";
                        for (auto &&item : condition)
                        {
                                ss << sep << item.first << "=" << t.quote(utils::any_to_string(item.second));
                                sep = " AND ";
                        }
                }
                ss << " LIMIT 1";

                SPDLOG_INFO("sql={}", ss.str());
                auto result = t.exec(ss.str(), "[FETCH FIRST]");
                t.commit();
                if (result.size() == 1)
                {
                        return {row_to_user_info(result[0]), true};
                }
                return {t_user_info{}, false};
        }

        pqxx::result t_user_info::remove(std::unordered_map<std::string_view, std::any> condition)
        {
                pqxx::work t(Database::get_conn());
                std::stringstream ss;
                ss << fmt::format("DELETE FROM {}", table_name());
                if (!condition.empty())
                {
                        ss << " WHERE";
                        std::string sep = " ";
                        for (const auto &item : condition)
                        {
                                ss << sep << item.first << "=" << t.quote(utils::any_to_string(item.second));
                                sep = " AND ";
                        }
                }

                SPDLOG_INFO("sql={}", ss.str());
                auto result = t.exec(ss.str(), "[REMOVE]");
                t.commit();
                return result;
        }

        pqxx::result t_user_info::update(std::unordered_map<std::string_view, std::any> condition, std::unordered_map<std::string_view, std::any> value, int limit)
        {
                if (condition.empty() || value.empty())
                {
                        return pqxx::result();
                }
                pqxx::work t(Database::get_conn());
                std::stringstream ss;
                ss << fmt::format("UPDATE {} SET", table_name());
                std::string sep = " ";
                for (auto &&item : value)
                {
                        ss << sep << item.first << "=" << t.quote(utils::any_to_string(item.second));
                        sep = ", ";
                }

                ss << " WHERE";
                sep = " ";
                for (auto &&item : condition)
                {
                        ss << sep << item.first << "=" << t.quote(utils::any_to_string(item.second));
                        sep = " AND ";
                }
                SPDLOG_INFO("sql={}", ss.str());
                auto result = t.exec(ss.str(), "[UPDATE]");
                t.commit();
                return result;
        }

        pqxx::result t_user_info::insert(std::unordered_map<std::string_view, std::any> value)
        {
                if (value.empty())
                {
                        return pqxx::result{};
                }
                pqxx::work t(Database::get_conn());
                std::stringstream ss;
                ss << fmt::format("INSERT INTO {}(", table_name());
                std::string sep = "";
                for (auto &&item : value)
                {
                        ss << sep << item.first;
                        sep = ", ";
                }
                ss << ") VALUES(";
                sep = "";
                for (auto &&item : value)
                {
                        ss << sep << t.quote(utils::any_to_string(item.second));
                        sep = ", ";
                }
                ss << ")";
                SPDLOG_INFO("sql={}", ss.str());
                auto result = t.exec(ss.str(), "[INSERT]");
                t.commit();
                return result;
        }

        pqxx::result t_user_info::insert(std::vector<std::unordered_map<std::string_view, std::any>> value)
        {
                if (value.empty())
                {
                        return pqxx::result{};
                }
                pqxx::work t(Database::get_conn());
                std::stringstream ss;
                ss << fmt::format("INSERT INTO {}(", table_name());
                std::string sep = "";
                for (auto &&item : value[0])
                {
                        ss << sep << item.first;
                        sep = ", ";
                }
                ss << ") VALUES";
                sep = "";
                for (auto &&item : value)
                {
                        ss << sep << "(";
                        std::string sep_ = "";
                        for (auto &&v : item)
                        {
                                ss << sep_ << t.quote(utils::any_to_string(v.second));
                                sep_ = ", ";
                        }
                        ss << ")";
                        sep = ", ";
                }
                SPDLOG_INFO("sql={}", ss.str());
                auto result = t.exec(ss.str(), "[BATCH INSERT]");
                t.commit();
                return result;
        }

} // namespace db
