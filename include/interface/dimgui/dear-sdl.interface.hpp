#pragma once

#include "dear-helper.hpp"
#include "interface.hpp"

#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"

#include <functional>
#include <memory>
#include <vector>

namespace CzaraEngine {
    class Czengine;
    class DearImGuiInterface : public Interface {
        public:
            DearImGuiInterface(std::shared_ptr<SDL_Window> &, std::shared_ptr<SDL_Renderer> &, std::function<bool(DearImGuiInterface&)> = DearHelper::sdlGuard);
            virtual ~DearImGuiInterface();
            virtual void addComponent(std::shared_ptr<Component> &component);
            virtual void addComponents(std::vector<std::shared_ptr<Component>> &components);
            virtual void newFrame();
            virtual void render();
            virtual void draw();
            virtual void drawInterface();
            virtual void processEvent(SDL_Event &event);
            friend class DearHelper;
        protected:
            std::weak_ptr<SDL_Window> m_window;
            std::weak_ptr<SDL_Renderer> m_renderer;
            std::vector<std::shared_ptr<Component>> m_components;
            std::function<bool(DearImGuiInterface&)> m_guard;
            bool m_sdl_initialized = false;
    };
}
