#include "dimgui-window.hpp"
#include "imgui.h"

namespace CzaraEngine {
    Panel::Panel(const PanelProps &props): m_properties(props) {
        m_render_flag = nullptr;
    }
    Panel::Panel(const PanelProps &props, const bool &render_flag): m_properties(props) {
        m_render_flag = new bool(render_flag);
        registerClosablePanel(this);
    }
    Panel::~Panel() {
        if (isClosablePanel(this)) {
            getClosablePanelMap().erase(this);
        }
        if (m_render_flag != nullptr) {
            delete m_render_flag;
            m_render_flag = nullptr;
        }
    }

    void Panel::beginComponent() {
        if (m_render_flag == nullptr || *m_render_flag) {
            if (children.size() > 0) {
                m_render_children = true;
            }
            ImGuiWindowFlags flags = generateFlags();
            ImGui::Begin(m_properties.id.c_str(), m_render_flag, flags);
        } else {
            m_render_children = false;
        }
    }
    void Panel::endComponent() {
        if (m_render_flag == nullptr || *m_render_flag) {
            ImGui::End();
        }
    }
    ImGuiWindowFlags Panel::generateFlags() {
        ImGuiWindowFlags flags = ImGuiWindowFlags_None;
        // if (m_properties.no_title) {
        //     flags |= ImGuiWindowFlags_NoTitleBar;
        // } if (m_properties.no_resize) {
        //     flags |= ImGuiWindowFlags_NoResize;
        // } if (m_properties.no_move) {
        //     flags |= ImGuiWindowFlags_NoMove;
        // } if (m_properties.no_vertical_scrollbar) {
        //     flags |= ImGuiWindowFlags_NoScrollbar;
        // } if (!m_properties.no_horizontal_scrollbar) { // Only negation
        //     flags |= ImGuiWindowFlags_HorizontalScrollbar;
        // } if (m_properties.no_mouse) {
        //     flags |= ImGuiWindowFlags_NoMouseInputs;
        // } if (m_properties.no_mouse_scroll) {
        //     flags |= ImGuiWindowFlags_NoScrollWithMouse;
        // } if (m_properties.no_collapse) {
        //     flags |= ImGuiWindowFlags_NoCollapse;
        // } if (m_properties.no_background) {
        //     flags |= ImGuiWindowFlags_NoBackground;
        // } if (m_properties.has_menu) {
        //     flags |= ImGuiWindowFlags_MenuBar;
        // } if (m_properties.no_nav) {
        //     flags |= ImGuiWindowFlags_NoNav;
        // } if (m_properties.no_nav_input) {
        //     flags |= ImGuiWindowFlags_NoNavInputs;
        // } if (m_properties.no_docking) {
        //     flags |= ImGuiWindowFlags_NoDocking;
        // } if (m_properties.no_decoration) {
        //     flags |= ImGuiWindowFlags_NoDecoration;
        // } if (m_properties.no_inputs) {
        //     flags |= ImGuiWindowFlags_NoInputs;
        // }
        return flags;
    }

    // Static inline functions
    std::map<void*, bool*>& Panel::getClosablePanelMap() {
        static std::map<void*, bool*> m_map;
        return m_map;
    }
    bool Panel::isClosablePanel(Component * component) {
        auto it = getClosablePanelMap().find(component);
        if (it == getClosablePanelMap().end()) {
            return false;
        }
        return true;
    }
    bool Panel::openClosablePanel(Component * component) {
        bool result = false;
        auto it = getClosablePanelMap().find(component);
        if (it != getClosablePanelMap().end()) {
            result = true;
            *(it->second) = true;
        }
        return result;
    }
    bool Panel::closeClosablePanel(Component * component) {
        bool result = false;
        auto it = getClosablePanelMap().find(component);
        if (it != getClosablePanelMap().end()) {
            result = true;
            *(it->second) = false;
        }
        return result;
    }
    bool Panel::toggleClosablePanel(Component * component) {
        bool result = false;
        auto it = getClosablePanelMap().find(component);
        if (it != getClosablePanelMap().end()) {
            result = true;
            *(it->second) = !(*(it->second));
        }
        return result;
    }
    void Panel::registerClosablePanel(Panel * panel) {
        bool * panel_flag = panel->m_render_flag;
        getClosablePanelMap()[panel] = panel_flag;
    }
}