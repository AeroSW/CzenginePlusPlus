#pragma once

#include "component.hpp"
#include "xml-interface-binding-macro.hpp"

namespace CzaraEngine {
    class Menu : public Component {
        public:
            Menu(const std::string &txt);
            virtual ~Menu();
        protected:
            virtual void beginComponent();
            virtual void endComponent();
        private:
            std::string m_txt;
    };
}
BIND_UX_COMPONENT(Menu, CzaraEngine::Menu, std::string);
