#pragma once

#include <string>
#include "sdl-window.hpp"
#include "inttypes.hpp"

namespace CzaraEngine {
    struct FileConfig {
        std::string name;
        std::string directory;
    };
    struct WindowConfig {
        ui32 width = 800;
        ui32 height = 640;
        ui32 x_window_offset = SdlWindow::X_CENTER;
        ui32 y_window_offset = SdlWindow::Y_CENTER;
    };
    struct RenderConfig {
        std::string api = "Vulkan";
    };
    struct LogFileConstraints {
        ui64 amount;
        std::string unit;
    };
    struct LogFileConfig : public FileConfig {
        std::string type;
        LogFileConstraints constraints;
    };
    struct CzengineAppConfig {
        std::string title;
        std::string author;
        WindowConfig window;
        RenderConfig render;
        FileConfig interface;
        LogFileConfig application_log;
        LogFileConfig error_log;
        LogFileConfig file_log;
    };
}