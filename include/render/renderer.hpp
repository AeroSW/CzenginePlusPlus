#pragma once

#include "inttypes.hpp"

#include <memory>
#include <string>

namespace CzaraEngine {
    class Renderer {
        public:
            virtual ~Renderer();
            virtual void drawColorRgb(const ui8 &red, const ui8 &green, const ui8 &blue, const ui8 &alpha) = 0;
            virtual void clearRenderBuffer() = 0;
            virtual void render() = 0;
        protected:
            Renderer(const std::string &dev_name = "");
            std::string m_saved_dev_name;
    };
}