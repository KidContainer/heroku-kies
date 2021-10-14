#include "cinatra.hpp"

namespace utils
{
    std::string get_ip(cinatra::request &req)
    {
        return req.get_conn<cinatra::NonSSL>()->remote_address();
    }
} // namespace utils
