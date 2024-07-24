#include "czengine-config-parser.hpp"
#include "app-logs.hpp"
#include "czengine.hpp"
#include <wtypes.h>

using namespace CzaraEngine;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
    CzengineConfigManager config_manager;
    const CzengineAppConfig& application_config = config_manager.getAppConfig();
    if (!Logger::createApplicationLog(application_config.application_log)
        || !Logger::createErrorLog(application_config.error_log)
        || !Logger::createFileLog(application_config.file_log)) {
        showErrorMessageBox("Czengine Initiation Alert", "Application instantiation workflow interrupted.");
        std::exit(1);
    }

    Czengine engine{config_manager.getAppConfig()};
    engine.sustainEventLoop();

    return 0;
}
