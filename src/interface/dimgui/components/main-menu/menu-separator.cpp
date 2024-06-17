#include "menu-separator.hpp"
#include "imgui.h"

namespace CzaraEngine {
    MenuSeparator::MenuSeparator() {}
    MenuSeparator::MenuSeparator(const std::string &txt) :
        m_txt(txt) {}
    MenuSeparator::~MenuSeparator() {}
    void MenuSeparator::beginComponent() {
        m_render_children = false;
        if (m_txt.size() > 0) {
            ImGui::SeparatorText(m_txt.c_str());
        } else {
            ImGui::Separator();
        }
    }
    void MenuSeparator::endComponent() {
    }
}