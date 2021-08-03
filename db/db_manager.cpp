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
            // std::terminate();
        }
        SPDLOG_INFO("psql_str={}", psql_str);
        pqxx::connection conn;
        // if(!conn.is_open()){
        //     std::terminate();
        // }
        return conn;
    }();
    
} // namespace db

