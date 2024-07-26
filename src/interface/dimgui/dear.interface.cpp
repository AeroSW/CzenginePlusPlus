#include "dear.interface.hpp"

#include "backends/imgui_impl_sdl2.h"

namespace CzaraEngine {
    DearInterface::DearInterface(const std::shared_ptr<SDL_Window> &window) :
        m_window(window), m_sdl_initialized(false), m_io(nullptr) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        m_io = std::make_unique<ImGuiIO>(ImGui::GetIO());
        m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        m_io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        m_io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        //sdlGuard(this);
    }
    DearInterface::~DearInterface() {
        ImGui::DestroyContext();
    }
    void DearInterface::addComponent(std::shared_ptr<Component> &component) {
        m_components.push_back(component);
    }
    void DearInterface::addComponents(std::vector<std::shared_ptr<Component>> &components) {
        m_components.insert(m_components.end(), components.begin(), components.end());
    }
    void DearInterface::processEvent(const SDL_Event &event) {
        if (sdlGuard(this)) return;
        ImGui_ImplSDL2_ProcessEvent(&event);
    }
    bool sdlGuard(DearInterface * interface) {
        if (interface->isInitialized()) return false;
        if (interface->isRendererUninitialized()) return true;
        interface->initInterfaceRendering();
        return false;
    }
}