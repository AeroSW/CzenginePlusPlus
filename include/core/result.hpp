#pragma once

#include "shared.hpp"

namespace CzaraEngine {
    template<typename T>
    struct Result {
        Shared<T> result;
        bool valid;
    };
}