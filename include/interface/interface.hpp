#pragma once

#include <memory>
#include <vector>
#include "component.hpp"
#include "SDL.h"

namespace CzaraEngine {
    class CzengineInterface {
        public:
            virtual ~CzengineInterface() {}
            virtual void addComponent(std::shared_ptr<Component> &component) = 0;
            virtual void addComponents(std::vector<std::shared_ptr<Component>> &components) = 0;
            virtual void newFrame() = 0;
            virtual void render() = 0;
            virtual void draw() = 0;
            virtual void drawInterface() = 0;
            virtual void processEvent(const SDL_Event &event) = 0;
        protected:
            CzengineInterface();
    };
}
