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
            pqxx::work wk(Database::get_conn());
            auto result = wk.exec(fmt::format("(SELECT EXISTS (SELECT table_name FROM information_schema.tables WHERE table_name ={}))",wk.quote(table_name)));
            if(result.empty()){
                return false;
            }
            return true;
        }catch(pqxx::broken_connection& err){
            SPDLOG_ERROR("exception has occured, {}", err.what());
            return false;
        }
    }


} // namespace db
