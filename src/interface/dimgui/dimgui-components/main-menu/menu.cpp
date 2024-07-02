#include "menu.hpp"
#include "imgui.h"

namespace CzaraEngine {
    Menu::Menu(const std::string &txt): m_txt(txt) {}
    Menu::~Menu() {}
    void Menu::beginComponent() {
        m_render_children = ImGui::BeginMenu(m_txt.c_str());
    }
    void Menu::endComponent() {
        ImGui::EndMenu();
    }
}