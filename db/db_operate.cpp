#include "db_operate.hpp"
#include "db_manager.hpp"

namespace db
{
    bool database_init(){
        //t_user_info
        return table_exists("t_user_info");
    }


    bool table_exists(std::string_view table_name){
        try{
            auto psql_str = std::getenv("DATABASE_URL");
            if(psql_str==nullptr){
                SPDLOG_ERROR("environment variable DATABASE_URL needed");
                std::terminate();
            }
            pqxx::connection conn{psql_str};
            if(!conn.is_open()){
                SPDLOG_ERROR("connection does not open");
                std::terminate();
            }
            SPDLOG_INFO("connection has established, url={}", psql_str);
            return true;
        }catch(std::exception& err){
            SPDLOG_ERROR("exception={}", err.what());
            return false;
        }
    }


} // namespace db
