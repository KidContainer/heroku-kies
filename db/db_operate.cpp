#include "db_operate.hpp"
#include "db_manager.hpp"
#include "t_user_info.hpp"
#include "t_blog.hpp"

namespace db
{
    void database_init()
    {
        //create t_user_info
        auto result = t_user_info::create_table();
        Database::print_result(result);

        //create t_blog
        result = t_blog::create_table();
        Database::print_result(result);
        
    }


} // namespace db
