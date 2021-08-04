#include "db_operate.hpp"
#include "db_manager.hpp"
#include "t_user_info.hpp"

namespace db
{
    void database_init()
    {
        //create t_user_info
        auto result = t_user_info::create_table();
        Database::print_result(result);

        //print out the current data
        
    }


} // namespace db
