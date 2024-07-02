#include "dimgui-text.hpp"
#include "imgui.h"

namespace CzaraEngine {
    DimguiText::DimguiText(const std::string &txt) : m_text(txt) {}
    DimguiText::~DimguiText() {}
    void DimguiText::beginComponent() {
        ImGui::Text(m_text.c_str());
    }
    void DimguiText::endComponent() {}
}