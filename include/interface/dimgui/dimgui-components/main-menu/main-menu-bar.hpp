#pragma once

#include "component.hpp"
#include "xml-interface-binding-macro.hpp"

namespace CzaraEngine {
    class MainMenuBar : public Component {
        public:
            MainMenuBar();
            virtual ~MainMenuBar();
        protected:
            virtual void beginComponent();
            virtual void endComponent();
    };
}
BIND_UX_COMPONENT(MainMenuBar, CzaraEngine::MainMenuBar);
