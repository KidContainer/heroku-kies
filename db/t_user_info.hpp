#ifndef DB_T_USER_INFO_HPP
#define DB_T_USER_INFO_HPP

#include <string>
#include <pqxx/pqxx>
#include <unordered_map>
#include <map>
#include <any>

namespace db
{
    class t_user_info{
        //Data
        public:
        std::int64_t id;
        std::string user_name;
        std::string password;
        std::int64_t create_time;
        std::string last_login;
        std::string email;
        std::string profile;

        private:
        static t_user_info row_to_user_info(pqxx::row res);


        public:
        static inline std::string table_name();

        static pqxx::result create_table();

        static pqxx::result delete_table();

        static std::vector<t_user_info> fetch(std::unordered_map<std::string_view, std::any> condition);

        static std::tuple<t_user_info,bool> fetch_first(std::unordered_map<std::string_view, std::any> condition);

        static pqxx::result remove(std::unordered_map<std::string_view, std::any> condition);

        static pqxx::result update(std::unordered_map<std::string_view, std::any> condition, std::unordered_map<std::string_view, std::any> value, int limit=0);

        static pqxx::result insert(std::map<std::string_view, std::any> value);

        static pqxx::result insert(std::vector<std::map<std::string_view, std::any>> value);
        
    };
    
} // namespace db



#endif