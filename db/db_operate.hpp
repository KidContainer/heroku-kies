#ifndef DB_DB_OPERATE_HPP
#define DB_DB_OPERATE_HPP

#include <string_view>

namespace db
{
    //Check if the table has been created and create one if it's not
    void database_init();

} // namespace db



#endif