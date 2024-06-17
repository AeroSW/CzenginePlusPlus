#pragma once

#include "interface.hpp"
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"
#include "shared.hpp"
#include "sdl-wrapper.hpp"

#include <vector>

namespace CzaraEngine {
    class SdlWindow;
    class DearImGuiInterface : public Interface {
        public:
            DearImGuiInterface(Shared<SdlWindowWrapper> &, Shared<SdlRendererWrapper> &);
            virtual ~DearImGuiInterface();
            virtual void addComponent(Shared<Component> &component);
            virtual void addComponents(std::vector<Shared<Component>> &components);
            virtual void newFrame();
            virtual void render();
            virtual void draw();
            virtual void drawInterface();
            virtual void processEvent(SDL_Event &event);
        protected:
            Shared<SdlWindowWrapper> m_window;
            Shared<SdlRendererWrapper> m_renderer;
            std::vector<Shared<Component>> m_components;
            bool m_sdl_initialized = false;
        private:
            bool sdlGuard();
    };
}
