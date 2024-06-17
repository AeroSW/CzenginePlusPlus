#pragma once

#include "component.hpp"
#include "xml-interface-binding-macro.hpp"

namespace CzaraEngine {
    class MenuSeparator : public Component {
        public:
            MenuSeparator();
            MenuSeparator(const std::string &txt);
            virtual ~MenuSeparator();
        protected:
            virtual void beginComponent();
            virtual void endComponent();
        private:
            std::string m_txt;
    };
}
BIND_UX_COMPONENT(Separator, CzaraEngine::MenuSeparator);
BIND_UX_COMPONENT(Separator_Text, CzaraEngine::MenuSeparator, std::string);
