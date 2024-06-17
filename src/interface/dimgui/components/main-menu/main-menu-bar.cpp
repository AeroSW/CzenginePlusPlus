#include "main-menu-bar.hpp"
#include "imgui.h"

namespace CzaraEngine {
    MainMenuBar::MainMenuBar() {}
    MainMenuBar::~MainMenuBar() {}
    void MainMenuBar::beginComponent() {
        m_render_children = ImGui::BeginMainMenuBar();
    }
    void MainMenuBar::endComponent() {
        ImGui::EndMainMenuBar();
    }
}