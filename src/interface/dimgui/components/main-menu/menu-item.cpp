#include "menu-item.hpp"
#include "imgui.h"

namespace CzaraEngine {
    MenuItem::MenuItem(const std::string &txt) :
        m_txt(txt), m_shortcut(nullptr), m_functor(nullptr) {}
    MenuItem::MenuItem(const std::string &txt, const std::string &shortcut) :
        m_txt(txt), m_shortcut(shortcut.c_str()), m_functor(nullptr) {}
    MenuItem::MenuItem(const std::string &txt, const std::string &shortcut, std::function<void()> functor) :
        m_txt(txt), m_shortcut(shortcut.c_str()), m_functor(functor) {}
    MenuItem::MenuItem(const std::string &txt, std::function<void()> functor) :
        m_txt(txt), m_shortcut(nullptr), m_functor(functor) {}
    MenuItem::~MenuItem() {}
    void MenuItem::beginComponent() {
        m_render_children = false;
        if (m_shortcut != nullptr && m_functor != nullptr) {
            ImGui::MenuItem(m_txt.c_str(), m_shortcut, &m_functor);
        } else if (m_shortcut != nullptr) {
            ImGui::MenuItem(m_txt.c_str(), m_shortcut);
        } else if (m_functor != nullptr) {
            ImGui::MenuItem(m_txt.c_str(), NULL, &m_functor);
        } else {
            ImGui::MenuItem(m_txt.c_str());
        }
    }
    void MenuItem::endComponent() {
    }
}