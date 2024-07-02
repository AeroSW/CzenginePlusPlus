#pragma once

#include "component.hpp"
#include "xml-interface-binding-macro.hpp"

namespace CzaraEngine {
    class Layer {
        public:
            Layer(const std::string &id);
            virtual ~Layer();
        protected:
            virtual void beginComponent();
            virtual void endComponent();
        private:
            std::string m_id;
    };
}

BIND_UX_COMPONENT(Layer, CzaraEngine::Layer, std::string);
