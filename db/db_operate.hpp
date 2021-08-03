#ifndef DB_DB_OPERATE_HPP
#define DB_DB_OPERATE_HPP

#include <string_view>
#include <spdlog/spdlog.h>

namespace db
{
    //Check if the table has been created and create one if it's not
    void database_init();


    //Excute one sql without caring about the result
    bool exec(std::string_view sql);

} // namespace db



#endif