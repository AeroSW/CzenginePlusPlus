#pragma once

#include <memory>
#include "window.hpp"
#include <vector>
#include "component.hpp"

namespace CzaraEngine {
    class Interface {
        public:
            virtual ~Interface() {}
            virtual void addComponent(std::shared_ptr<Component> &component) = 0;
            virtual void addComponents(std::vector<std::shared_ptr<Component>> &components) = 0;
            virtual void newFrame() = 0;
            virtual void render() = 0;
            virtual void draw() = 0;
            virtual void drawInterface() = 0;
        protected:
            Interface();
    };
}
