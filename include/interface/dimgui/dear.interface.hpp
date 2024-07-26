#pragma once

#include "interface.hpp"

#include <functional>
#include <memory>
#include <vector>

#include "imgui.h"

namespace CzaraEngine {
    class DearInterface : public CzengineInterface {
        public:
            virtual ~DearInterface();
            virtual void addComponent(std::shared_ptr<Component> &component);
            virtual void addComponents(std::vector<std::shared_ptr<Component>> &components);
            virtual void newFrame() = 0;
            virtual void render() = 0;
            virtual void draw() = 0;
            virtual void drawInterface() = 0;
            virtual bool isRendererUninitialized() = 0;
            virtual bool isInitialized() = 0;
            virtual void initInterfaceRendering() = 0;
            virtual void processEvent(const SDL_Event &event);

        protected:
            DearInterface(const std::shared_ptr<SDL_Window> &window);

            bool m_sdl_initialized = false;
            std::unique_ptr<ImGuiIO> m_io;

            std::weak_ptr<SDL_Window> m_window;
            std::vector<std::shared_ptr<Component>> m_components;

    };
    bool sdlGuard(DearInterface*);
}
