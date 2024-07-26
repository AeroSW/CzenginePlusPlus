#include "dear-sdl.interface.hpp"
#include "utility.hpp"
#include <iostream>

#include "backends/imgui_impl_sdl2.h"

namespace CzaraEngine {
    
    DearSdlInterface::DearSdlInterface(const std::shared_ptr<SDL_Window> &window, const std::shared_ptr<Sdl2Renderer> &renderer) :
        DearInterface(window), m_renderer(renderer) {
            ImGui_ImplSDL2_InitForSDLRenderer(window.get(), renderer->getSdlRenderer().get());
        }
    DearSdlInterface::~DearSdlInterface() {
        ImGui_ImplSDLRenderer2_Shutdown();
        ImGui_ImplSDL2_Shutdown();
    }
    void DearSdlInterface::newFrame() {
        if (sdlGuard(this)) return;
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
    }
    void DearSdlInterface::render() {
        if (sdlGuard(this)) return;
        ImGui::Render();
    }
    void DearSdlInterface::draw() {
        if (sdlGuard(this)) return;
        std::shared_ptr<Sdl2Renderer> renderer = m_renderer.lock();
        ImGui_ImplSDLRenderer2_RenderDrawData(
            ImGui::GetDrawData(),
            renderer->getSdlRenderer().get()
        );
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
    void DearSdlInterface::drawInterface() {
        for (std::shared_ptr<Component> component : m_components) {
            component->renderComponent();
        }
    }
    bool DearSdlInterface::isRendererUninitialized() {
        return isWeakPtrUnitialized(m_renderer);
    }
    void DearSdlInterface::initInterfaceRendering() {
        std::shared_ptr<SDL_Window> window = m_window.lock();
        std::shared_ptr<Sdl2Renderer> renderer = m_renderer.lock();
        ImGui_ImplSDL2_InitForSDLRenderer(window.get(), renderer->getSdlRenderer().get());
        ImGui_ImplSDLRenderer2_Init(renderer->getSdlRenderer().get());
        m_sdl_initialized = true;
    }
    bool DearSdlInterface::isInitialized() {
        return m_sdl_initialized;
    }
}
