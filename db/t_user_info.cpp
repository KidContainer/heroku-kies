#include "t_user_info.hpp"
#include "db_manager.hpp"
#include <spdlog/spdlog.h>

namespace db
{
        std::string t_user_info::table_name()
        {
                return "t_user_info";
        }

        pqxx::result t_user_info::create_table()
        {
                pqxx::work t(Database::get_conn());
                auto result = t.exec(fmt::format(R"sql(CREATE TABLE IF NOT EXISTS {} (
                        id              SERIAL PRIMARY KEY COMMENT 'User ID',
                        user_name       VARCHAR(200) NOT NULL COMMENT 'User Name',
                        password        VARCHAR(200) NOT NULL COMMENT 'User Password which is not encrypted',
                        create_time     INTEGER NOT NULL COMMENT 'The TimeStamp when user create his acount',
                        last_login      TIMESTAMP DEFAULT CURRENT_TIMESTAMP COMMENT 'The TimeStamp when user last logged in'
                ))sql", table_name()),"[CREATE TABLE]");
                t.commit();
                return result;
        }

        pqxx::result t_user_info::delete_table()
        {
                pqxx::work t(Database::get_conn());
                auto result = t.exec(fmt::format(R"sql(DROP TABLE {})sql", table_name()),"[DROP TABLE]");
                t.commit();
                return result;
        }

        // std::vector<t_user_info> t_user_info::fetch(std::unordered_map<std::string_view, std::any> condition)
        // {
        //         std::stringstream ss;
        //         ss<<fmt::format(R"sql(SELECT * FROM {} )sql", table_name());
        //         if(condition.size()!=0){
        //                 ss<<R"sql(WHERE )sql";
        //                 for(auto&& item : condition){
                                
        //                 }
        //         }
        // }

        // t_user_info t_user_info::fetch_first(std::unordered_map<std::string_view, std::any> condition)
        // {
        // }

        // pqxx::result t_user_info::remove(std::unordered_map<std::string_view, std::any> condition, int limit)
        // {
        // }

        // pqxx::result t_user_info::update(std::unordered_map<std::string_view, std::any> condition, std::unordered_map<std::string_view, std::any> value, int limit = 0)
        // {
        // }

        // pqxx::result t_user_info::insert(std::unordered_map<std::string_view, std::any> value)
        // {
        // }

        // pqxx::result t_user_info::insert(std::vector<std::unordered_map<std::string_view, std::any>> value)
        // {
        // }

} // namespace db
