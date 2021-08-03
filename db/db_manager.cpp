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
            throw std::runtime_error("DATABASE_URL is missing");
        }
        pqxx::connection conn(psql_str);
        if(!conn.is_open()){
            throw std::runtime_error("connection is not established");
        }
        return conn;
    }();
    
} // namespace db

