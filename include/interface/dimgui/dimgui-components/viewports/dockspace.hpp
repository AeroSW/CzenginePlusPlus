#pragma once

#include "component.hpp"
#include "xml-interface-binding-macro.hpp"

namespace CzaraEngine {
    class DockingEnabler : public Component {
        public:
            DockingEnabler();
            virtual ~DockingEnabler();
        protected:
            virtual void beginComponent();
            virtual void endComponent();
    };
}
BIND_UX_COMPONENT(EnableDocking, CzaraEngine::DockingEnabler);