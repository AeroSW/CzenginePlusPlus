#pragma once

#include "singleton.hpp"

namespace CzaraEngine {
    enum class AppState {
        __DEBUG__,
        __PROD__
    };
    struct AppConfiguration {
        AppState app_state;
        const char * default_log_dir = "./logs";
        AppConfiguration(AppState state) : app_state(state) {}
        ~AppConfiguration() {}
    };
    extern Singleton<AppConfiguration> app_config;
}
