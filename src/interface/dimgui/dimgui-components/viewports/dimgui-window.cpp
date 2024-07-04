#include "dimgui-window.hpp"
#include "imgui.h"

#include <iomanip>
#include <functional>
#include <sstream>

namespace CzaraEngine {
    Panel::Panel(const PanelProps &props) {
        m_properties = props;
        m_render_flag = nullptr;
    }
    Panel::Panel(const PanelProps &props, const bool &render_flag) {
        m_properties = props;
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
        if (m_properties.no_title) {
            flags |= ImGuiWindowFlags_NoTitleBar;
        }
        if (m_properties.no_resize) {
            flags |= ImGuiWindowFlags_NoResize;
        }
        if (m_properties.no_move) {
            flags |= ImGuiWindowFlags_NoMove;
        }
        if (m_properties.no_vertical_scrollbar) {
            flags |= ImGuiWindowFlags_NoScrollbar;
        }
        if (m_properties.horizontal_scrollbar) { // Only negation
            flags |= ImGuiWindowFlags_HorizontalScrollbar;
        }
        if (m_properties.no_mouse) {
            flags |= ImGuiWindowFlags_NoMouseInputs;
        }
        if (m_properties.no_mouse_scroll) {
            flags |= ImGuiWindowFlags_NoScrollWithMouse;
        }
        if (m_properties.no_collapse) {
            flags |= ImGuiWindowFlags_NoCollapse;
        }
        if (m_properties.no_background) {
            flags |= ImGuiWindowFlags_NoBackground;
        }
        if (m_properties.has_menu) {
            flags |= ImGuiWindowFlags_MenuBar;
        }
        if (m_properties.no_nav) {
            flags |= ImGuiWindowFlags_NoNav;
        }
        if (m_properties.no_nav_input) {
            flags |= ImGuiWindowFlags_NoNavInputs;
        }
        if (m_properties.no_docking) {
            flags |= ImGuiWindowFlags_NoDocking;
        }
        if (m_properties.no_decoration) {
            flags |= ImGuiWindowFlags_NoDecoration;
        }
        if (m_properties.no_inputs) {
            flags |= ImGuiWindowFlags_NoInputs;
        }
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

    std::function<std::string(bool)> boolToString = [](bool arg) { return (arg) ? "True" : "False"; };
    std::ostream& operator<<(std::ostream& stream, PanelProps& props) {
        std::ostringstream msg;
        msg << "Panel Properties:\n";
        msg << "\tid: " << props.id << "\n";
        msg << "\tNo Title:  " << boolToString(props.no_title) << "\n";
        msg << "\tNo Resize: " << boolToString(props.no_resize) << "\n";
        msg << "\tNo Move:   " << boolToString(props.no_move) << "\n";
        msg << "\tNo Mouse:  " << boolToString(props.no_mouse) << "\n";
        msg << "\tNo Vertical Scrollbar: " << boolToString(props.no_vertical_scrollbar) << "\n";
        msg << "\tHorizontal Scrollbar:  " << boolToString(props.horizontal_scrollbar) << "\n";
        msg << "\tNo Mouse Scroll: " << boolToString(props.no_mouse_scroll) << "\n";
        msg << "\tNo Collapse:     " << boolToString(props.no_collapse) << "\n";
        msg << "\tNo Background:   " << boolToString(props.no_background) << "\n";
        msg << "\tHas Menu: " << boolToString(props.has_menu) << "\n";
        msg << "\tNo Nav:   " << boolToString(props.no_nav) << "\n";
        msg << "\tNo Nav Input:  " << boolToString(props.no_nav_input) << "\n";
        msg << "\tNo Docking:    " << boolToString(props.no_docking) << "\n";
        msg << "\tNo Decoration: " << boolToString(props.no_decoration) << "\n";
        msg << "\tNo Inputs: " << boolToString(props.no_inputs) << "\n";
        return stream << msg.str();
    }
    std::ostream& operator<<(std::ostream& stream, const PanelProps& props) {
        std::ostringstream msg;
        msg << "Panel Properties:\n";
        msg << "\tid: " << props.id << "\n";
        msg << "\tNo Title:  " << boolToString(props.no_title) << "\n";
        msg << "\tNo Resize: " << boolToString(props.no_resize) << "\n";
        msg << "\tNo Move:   " << boolToString(props.no_move) << "\n";
        msg << "\tNo Mouse:  " << boolToString(props.no_mouse) << "\n";
        msg << "\tNo Vertical Scrollbar: " << boolToString(props.no_vertical_scrollbar) << "\n";
        msg << "\tHorizontal Scrollbar:  " << boolToString(props.horizontal_scrollbar) << "\n";
        msg << "\tNo Mouse Scroll: " << boolToString(props.no_mouse_scroll) << "\n";
        msg << "\tNo Collapse:     " << boolToString(props.no_collapse) << "\n";
        msg << "\tNo Background:   " << boolToString(props.no_background) << "\n";
        msg << "\tHas Menu: " << boolToString(props.has_menu) << "\n";
        msg << "\tNo Nav:   " << boolToString(props.no_nav) << "\n";
        msg << "\tNo Nav Input:  " << boolToString(props.no_nav_input) << "\n";
        msg << "\tNo Docking:    " << boolToString(props.no_docking) << "\n";
        msg << "\tNo Decoration: " << boolToString(props.no_decoration) << "\n";
        msg << "\tNo Inputs: " << boolToString(props.no_inputs) << "\n";
        return stream << msg.str();
    }
    std::ostream& operator<<(std::ostream& stream, Panel& panel) {
        std::ostringstream msg;
        std::string render_flag = (panel.m_render_flag == nullptr) ? "nullptr" : boolToString(*panel.m_render_flag);
        msg << "Panel:\n";
        msg << "\tRender Flag: " << render_flag << "\n";
        msg << std::setw(34) << panel.m_properties << "\n";
        return stream << msg.str();
    }
}