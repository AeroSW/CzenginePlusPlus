#include "czengine-config-parser.hpp"
#include "app-logs.hpp"
#include "sdl-window.hpp"
#include <wtypes.h>

using namespace CzaraEngine;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
    CzengineConfigManager config_manager;
    const CzengineAppConfig& application_config = config_manager.getAppConfig();
    if (!Logger::createApplicationLog(application_config.application_log)
        || !Logger::createErrorLog(application_config.error_log)
        || !Logger::createFileLog(application_config.file_log)) {
        SdlWindow::showErrorMessageBox("Czengine Initiation Alert", "Application instantiation workflow interrupted.");
        std::exit(1);
    }

    SdlWindow m_window{WindowProperties{
        800,
        600,
        SdlWindow::X_CENTER,
        SdlWindow::Y_CENTER,
        config_manager.getAppConfig().title
    }};

    return 0;
}
