#include "main-menu-bar.hpp"

namespace CzaraEngine {
    MainMenuBar::MainMenuBar() {}
    MainMenuBar::~MainMenuBar() {}
    void MainMenuBar::beginComponent() {
        renderChildren = ImGui::BeginMainMenuBar();
    }
    void MainMenuBar::endComponent() {
        ImGui::EndMainMenuBar();
    }
}