#pragma once

#include <memory>

namespace CzaraEngine {
    template<typename T>
    struct Result {
        std::shared_ptr<T> result;
        bool valid;
    };
}