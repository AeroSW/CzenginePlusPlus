#pragma once

#include "component.hpp"
#include <functional>
#include "xml-interface-binding-macro.hpp"

namespace CzaraEngine {
    class MenuItem : public Component {
        public:
            MenuItem(const std::string &txt);
            MenuItem(const std::string &txt, const std::string &shortcut);
            MenuItem(const std::string &txt, std::function<void()> functor);
            MenuItem(const std::string &txt, const std::string &shortcut, std::function<void()> functor);
            virtual ~MenuItem();
        protected:
            virtual void beginComponent();
            virtual void endComponent();
        private:
            std::string m_txt;
            const char * m_shortcut;
            std::function<void()> m_functor;
    };
}
BIND_UX_COMPONENT(MenuItem, CzaraEngine::MenuItem, std::string);
BIND_UX_COMPONENT(MenuItem_Shortcut, CzaraEngine::MenuItem, std::string, std::string);
BIND_UX_COMPONENT(MenuItem_Callback, CzaraEngine::MenuItem, std::string, std::function<void()>);
BIND_UX_COMPONENT(MenuItem_Shortcut_Callback, CzaraEngine::MenuItem, std::string, std::string, std::function<void()>);
