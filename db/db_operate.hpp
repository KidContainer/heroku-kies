#ifndef DB_DB_OPERATE_HPP
#define DB_DB_OPERATE_HPP

#include <string_view>
#include <spdlog/spdlog.h>

namespace db
{
    //Check if the table has been created and create one if it's not
    bool database_init();


    //Check if the table exists
    bool table_exists(std::string_view table_name);

} // namespace db



#endif