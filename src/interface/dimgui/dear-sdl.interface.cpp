#include "dear-sdl.interface.hpp"
#include <iostream>

#include "backends/imgui_impl_sdl2.h"

namespace CzaraEngine {
    
    DearImGuiInterface::DearImGuiInterface(std::shared_ptr<SDL_Window> &window, std::shared_ptr<SDL_Renderer> &renderer, std::function<bool(DearImGuiInterface&)> guard) :
    Interface(), m_sdl_initialized(false), m_window(window), m_renderer(renderer), m_guard(guard) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io{ImGui::GetIO()};
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        m_guard(*this);
    }
    DearImGuiInterface::~DearImGuiInterface() {
        ImGui_ImplSDLRenderer2_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
    }
    void DearImGuiInterface::addComponent(std::shared_ptr<Component> &component) {
        m_components.push_back(component);
    }
    void DearImGuiInterface::addComponents(std::vector<std::shared_ptr<Component>> &components) {
        m_components.insert(m_components.end(), components.begin(), components.end());
    }
    void DearImGuiInterface::newFrame() {
        if (m_guard(*this)) return;
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
    }
    void DearImGuiInterface::render() {
        if (m_guard(*this)) return;
        ImGui::Render();
    }
    void DearImGuiInterface::draw() {
        if (m_guard(*this)) return;
        std::shared_ptr<SDL_Renderer> renderer = m_renderer.lock();
        ImGui_ImplSDLRenderer2_RenderDrawData(
            ImGui::GetDrawData(),
            renderer.get()
        );
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
    void DearImGuiInterface::drawInterface() {
        for (std::shared_ptr<Component> component : m_components) {
            component->renderComponent();
        }
    }
    void DearImGuiInterface::processEvent(SDL_Event &event) {
        if (m_guard(*this)) return;
        ImGui_ImplSDL2_ProcessEvent(&event);
    }
}
