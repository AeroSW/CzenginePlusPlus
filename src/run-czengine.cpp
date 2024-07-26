#include "czengine-config-parser.hpp"
#include "czengine-ux-file-parser.hpp"
#include "app-logs.hpp"
#include "czengine.hpp"
#include "dear-sdl.interface.hpp"
#include "dear-vulkan.interface.hpp"
#include <wtypes.h>
#include <filesystem>

namespace fs = std::filesystem;

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
    fs::path dir{application_config.ux.directory};
    fs::path name{application_config.ux.name};
    CzengineUxFileParser uxParser{dir / name};
    std::vector<std::shared_ptr<CzaraEngine::Component>> components = uxParser.processFile();
    Czengine engine{config_manager.getAppConfig()};
    std::shared_ptr<CzengineInterface> czinterface = nullptr;
    std::shared_ptr<SDL_Window> window = engine.getWindow();
    if (application_config.render.api == "Vulkan") {
        std::shared_ptr<SdlVulkanRenderer> vulkan_renderer = std::static_pointer_cast<SdlVulkanRenderer>(engine.getRenderer());
        czinterface = std::make_shared<DearVulkanInterface>(window, vulkan_renderer);
    } else if (application_config.render.api == "SDL2") {
        std::shared_ptr<Sdl2Renderer> sdl_renderer = std::static_pointer_cast<Sdl2Renderer>(engine.getRenderer());
        czinterface = std::make_shared<DearSdlInterface>(window, sdl_renderer);
    }
    if (czinterface) {
        czinterface->addComponents(components);
        engine.addInterface(czinterface);
    }
    engine.sustainEventLoop();

    return 0;
}
