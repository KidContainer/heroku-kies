#include "db_operate.hpp"
#include "db_manager.hpp"

namespace db
{
    void database_init()
    {
        //create t_user_info
        if(auto result = exec(R"sql(CREATE TABLE IF NOT EXISTS user_info (
            id              SERIAL PRIMARY KEY,
            user_name       VARCHAR(200) NOT NULL,
            password        VARCHAR(200) NOT NULL,
            create_time     INTEGER NOT NULL,
            last_login      TIMESTAMP DEFAULT CURRENT_TIMESTAMP
        ))sql"); !result){
            SPDLOG_ERROR("Error when check if table user_info exists");
        }
        

    }

    bool exec(std::string_view sql)
    {
        try
        {
            pqxx::work t(Database::get_conn());
            t.exec(sql);
            return true;
        }
        catch (std::exception &err)
        {
            SPDLOG_ERROR("exception has occured, {}", err.what());
            return false;
        }
    }

} // namespace db
