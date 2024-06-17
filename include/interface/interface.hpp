#pragma once

#include "shared.hpp"
#include "window.hpp"
#include <vector>
#include "component.hpp"

namespace CzaraEngine {
    class Interface {
        public:
            virtual ~Interface() {}
            virtual void addComponent(Shared<Component> &component) = 0;
            virtual void addComponents(std::vector<Shared<Component>> &components) = 0;
            virtual void newFrame() = 0;
            virtual void render() = 0;
            virtual void draw() = 0;
            virtual void drawInterface() = 0;
        protected:
            Interface();
    };
}
