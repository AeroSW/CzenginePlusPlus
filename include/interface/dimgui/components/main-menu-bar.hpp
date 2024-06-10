#pragma once

#include "dimgui-interface.hpp"
#include "component.hpp"

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