#include "container.hpp"

namespace utils
{
    std::unordered_map<std::string_view, std::any> s_map_to_sv_map(const std::unordered_map<std::string, std::any>& m){
        std::unordered_map<std::string_view, std::any> result;
        result.reserve(m.size());
        for(const auto& item : m){
            result.insert({item.first, item.second});
        }
        return result;
    }

} // namespace utils
