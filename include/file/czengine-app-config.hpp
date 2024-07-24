#pragma once

#include <string>
#include "czengine.hpp"
#include "inttypes.hpp"

namespace CzaraEngine {
    ui32& getCenterXPos();
    ui32& getCenterYPos();
    struct FileConfig {
        std::string name;
        std::string directory;
    };
    struct WindowConfig {
        ui32 width = 800;
        ui32 height = 640;
        ui32 x_window_offset = getCenterXPos();
        ui32 y_window_offset = getCenterYPos();
    };
    struct RenderConfig {
        std::string api = "Vulkan";
        std::string device = "";
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