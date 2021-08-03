#include "db_operate.hpp"
#include "db_manager.hpp"

namespace db
{
    bool database_init(){
        //t_user_info
        return table_exists("t_user_info");
    }


    bool table_exists(std::string_view table_name){
        // pqxx::work wk(Database::get_conn());
        // auto result = wk.exec(R"(SELECT EXISTS (SELECT table_name FROM information_schema.tables WHERE table_name =)"+wk.quote(table_name));
        // if(result.empty()){
        //     return false;
        // }
        Database::get_conn();
        return true;
    }


} // namespace db
