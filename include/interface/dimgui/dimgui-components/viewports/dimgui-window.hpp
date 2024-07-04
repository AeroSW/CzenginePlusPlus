#pragma once

#include "imgui.h"
#include "component.hpp"
#include "xml-interface-binding-macro.hpp"

#include <ostream>

namespace CzaraEngine {
    struct PanelProps {
        std::string id;
        bool no_title = false;
        bool no_resize = false;
        bool no_move = false;
        bool no_vertical_scrollbar = false;
        bool horizontal_scrollbar = false;
        bool no_mouse_scroll = false;
        bool no_collapse = false;
        bool no_background = false;
        bool no_mouse = false;
        bool has_menu = false;
        bool no_nav = false;
        bool no_nav_input = false;
        bool no_docking = false;
        bool no_decoration = false;
        bool no_inputs = false;
    };

    std::ostream& operator<<(std::ostream& stream, const PanelProps& props);
    std::ostream& operator<<(std::ostream& stream, PanelProps& props);
    
    class Panel : public Component {
        public:
            Panel(const PanelProps &props);
            Panel(const PanelProps &props, const bool &render_flag);
            virtual ~Panel();

            static inline bool isClosablePanel(Component *);
            static inline bool openClosablePanel(Component *);
            static inline bool closeClosablePanel(Component *);
            static inline bool toggleClosablePanel(Component *);
            friend std::ostream& operator<<(std::ostream& stream, Panel& panel);

        protected:
            virtual void beginComponent();
            virtual void endComponent();

        private:
            ImGuiWindowFlags generateFlags();
            bool * m_render_flag = nullptr;
            PanelProps m_properties;
            
            static inline void registerClosablePanel(Panel *);
            static inline std::map<void*, bool*>& getClosablePanelMap();
    };
}

BIND_UX_COMPONENT(Panel, CzaraEngine::Panel, CzaraEngine::PanelProps);
BIND_UX_COMPONENT(Panel_Bool, CzaraEngine::Panel, CzaraEngine::PanelProps, bool);
