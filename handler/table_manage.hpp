#ifndef HANDLER_TABLE_MANAGE_HPP
#define HANDLER_TABLE_MANAGE_HPP

#include <cinatra.hpp>

namespace handler
{
    //Directly operate on the t_user_info table
    void t_user_info_op(cinatra::request &req, cinatra::response &res);

    //Directly operate on the t_blog table
    void t_blog_op(cinatra::request &req, cinatra::response &res);

} // namespace handler

#endif