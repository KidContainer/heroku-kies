#ifndef HANDLER_HANDLER_HPP
#define HANDLER_HANDLER_HPP

#include <cinatra.hpp>

namespace handler{
    void home_page(cinatra::request& req, cinatra::response& res);
    void upload_file(cinatra::request& req, cinatra::response& res);
} // namespace handler

#endif