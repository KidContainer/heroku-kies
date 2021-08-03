#ifndef HANDLER_HANDLER_HPP
#define HANDLER_HANDLER_HPP

#include <cinatra.hpp>

namespace handler
{
    void upload_file(cinatra::request &req, cinatra::response &res);

    void debug_func(cinatra::request &req, cinatra::response &res);
} // namespace handler

#endif