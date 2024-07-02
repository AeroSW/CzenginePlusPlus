#pragma once

#include "component.hpp"
#include "xml-interface-binding-macro.hpp"

namespace CzaraEngine {
    class DimguiText : public Component {
        public:
            DimguiText(const std::string &txt);
            virtual ~DimguiText();
        protected:
            virtual void beginComponent();
            virtual void endComponent();
        private:
            std::string m_text;
    };
}

BIND_UX_COMPONENT(Text, CzaraEngine::DimguiText, std::string);
