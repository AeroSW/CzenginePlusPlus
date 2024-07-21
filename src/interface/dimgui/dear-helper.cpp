#include "dear-helper.hpp"
#include "dear-sdl.interface.hpp"
#include "utility.hpp"
namespace CzaraEngine {
    bool DearHelper::sdlGuard(DearImGuiInterface &interface) {
        if (interface.m_sdl_initialized) return false;
        if (isWeakPtrUnitialized(interface.m_renderer)) return true;
        std::shared_ptr<SDL_Window> window = interface.m_window.lock();
        std::shared_ptr<SDL_Renderer> renderer = interface.m_renderer.lock();
        ImGui_ImplSDL2_InitForSDLRenderer(window.get(), renderer.get());
        ImGui_ImplSDLRenderer2_Init(renderer.get());
        interface.m_sdl_initialized = true;
        return false;
    }
    bool DearHelper::vulkanGuard(DearImGuiInterface &interface) {
        if (interface.m_sdl_initialized) return false;
        if (isWeakPtrUnitialized(interface.m_renderer)) return true;
        std::shared_ptr<SDL_Window> window = interface.m_window.lock();
        ImGui_ImplSDL2_InitForVulkan(window.get());
        interface.m_sdl_initialized = true;
        return false;
    }
}