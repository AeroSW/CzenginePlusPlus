#include "log-utils.hpp"

namespace CzaraEngine {
    std::map<std::string, ui64>& getByteSizeMap() {
        static std::map<std::string, ui64> m_byte_size_map = {
            {"B", 1},
            {"KB", 1000},
            {"MB", 1000000},
            {"GB", 1000000000},
            {"TB", 1000000000000},
            {"PB", 1000000000000000}
        };
        return m_byte_size_map;
    }
    std::map<std::string, ui64>& getSecondsSizeMap() {
        static std::map<std::string, ui64> m_minute_size_map = {
            {"seconds", 1},
            {"minutes", 60},
            {"hours", 3600},
            {"days", 86400},
            {"weeks", 604800}
        };
        return m_minute_size_map;
    }
}