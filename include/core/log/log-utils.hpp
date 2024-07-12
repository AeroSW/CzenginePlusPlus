#pragma once

#include "inttypes.hpp"
#include <map>
#include <string>

namespace CzaraEngine {
    std::map<std::string, ui64>& getByteSizeMap();
    std::map<std::string, ui64>& getSecondsSizeMap();
}