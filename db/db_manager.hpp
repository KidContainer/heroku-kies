#ifndef DB_DB_MANAGER_HPP
#define DB_DB_MANAGER_HPP

#include <pqxx/pqxx>


namespace db
{
    class Database{
        static thread_local pqxx::connection conn;
        
        public:
            static pqxx::connection& get_conn();
        
    };
} // namespace db

#endif