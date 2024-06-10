#pragma once

#include "shared.hpp"
#include "window.hpp"

namespace CzaraEngine {
    class Interface {
        public:
            virtual ~Interface() {}
            virtual void newFrame() = 0;
            virtual void render() = 0;
            virtual void draw() = 0;
        protected:
            Interface();
    };
}
