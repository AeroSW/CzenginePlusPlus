#pragma once

#include <memory>
#include <string>
#include <vector>
#include "inttypes.hpp"
#include <memory>

namespace CzaraEngine {
    struct WindowProperties {
        ui32 width;
        ui32 height;
        ui32 x_window_offset;
        ui32 y_window_offset;
        std::string name;
    };

    class Window {
        public:
            virtual Window& operator=(const Window& window);
            Window& operator=(const Window&& window) = delete;

            virtual bool addChild(const WindowProperties& child_properties) = 0;
            virtual bool isOpen() = 0;
            virtual ~Window();

        protected:
            ui32 width;
            ui32 height;
            ui32 x_window_offset;
            ui32 y_window_offset;
            std::string name;
            std::vector<std::shared_ptr<Window>> children;

            Window();
            Window(const WindowProperties& properties);
            Window(const Window& window);
            Window(const Window&& window) = delete;

        private:
            virtual void clearChildrenVector();
    };
}
