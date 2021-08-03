#include "db_manager.hpp"
#include <spdlog/spdlog.h>
namespace db
{
    pqxx::connection& Database::get_conn(){
        return conn;
    }

    //Initialize the connection, if failed, the program should crash
    thread_local pqxx::connection Database::conn = []() noexcept{
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
        return conn;
    }();
    
} // namespace db

