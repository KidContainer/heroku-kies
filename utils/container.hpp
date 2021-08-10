#ifndef UTILS_CONTAINER_HPP
#define UTILS_CONTAINER_HPP

#include <string>
#include <string_view>
#include <any>
#include <unordered_map>

namespace utils
{
    std::unordered_map<std::string_view, std::any> s_map_to_sv_map(const std::unordered_map<std::string, std::any>& m);
} // namespace utils


#endif