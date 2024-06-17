#include "dimgui-interface.hpp"
#include <iostream>

namespace CzaraEngine {
    
    DearImGuiInterface::DearImGuiInterface(Shared<SdlWindowWrapper> &window, Shared<SdlRendererWrapper> &renderer) :
    Interface(), m_sdl_initialized(false), m_window(window), m_renderer(renderer) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io{ImGui::GetIO()};
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        sdlGuard();
    }
    DearImGuiInterface::~DearImGuiInterface() {
        ImGui_ImplSDLRenderer2_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
    }
    void DearImGuiInterface::addComponent(Shared<Component> &component) {
        m_components.push_back(component);
    }
    void DearImGuiInterface::addComponents(std::vector<Shared<Component>> &components) {
        m_components.insert(m_components.end(), components.begin(), components.end());
    }
    void DearImGuiInterface::newFrame() {
        if (sdlGuard()) return;
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
    }
    void DearImGuiInterface::render() {
        if (sdlGuard()) return;
        ImGui::Render();
    }
    void DearImGuiInterface::draw() {
        if (sdlGuard()) return;
        ImGui_ImplSDLRenderer2_RenderDrawData(
            ImGui::GetDrawData(),
            m_renderer->get()
        );
    }
    void DearImGuiInterface::drawInterface() {
        for (Shared<Component> component : m_components) {
            component.get()->renderComponent();
        }
    }
    void DearImGuiInterface::processEvent(SDL_Event &event) {
        if (sdlGuard()) return;
        ImGui_ImplSDL2_ProcessEvent(&event);
    }
    bool DearImGuiInterface::sdlGuard() {
        if (m_sdl_initialized) return false;
        if (!m_renderer->get()) return true;
        ImGui_ImplSDL2_InitForSDLRenderer(m_window->get(), m_renderer->get());
        ImGui_ImplSDLRenderer2_Init(m_renderer->get());
        m_sdl_initialized = true;
        return false;
    }
}
