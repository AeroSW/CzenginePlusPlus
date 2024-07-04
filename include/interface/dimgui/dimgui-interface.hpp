#pragma once

#include "interface.hpp"
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"
#include <memory>
#include "sdl-wrapper.hpp"

#include <vector>

namespace CzaraEngine {
    class SdlWindow;
    class DearImGuiInterface : public Interface {
        public:
            DearImGuiInterface(std::shared_ptr<SdlWindowWrapper> &, std::shared_ptr<SdlRendererWrapper> &);
            virtual ~DearImGuiInterface();
            virtual void addComponent(std::shared_ptr<Component> &component);
            virtual void addComponents(std::vector<std::shared_ptr<Component>> &components);
            virtual void newFrame();
            virtual void render();
            virtual void draw();
            virtual void drawInterface();
            virtual void processEvent(SDL_Event &event);
        protected:
            std::shared_ptr<SdlWindowWrapper> m_window;
            std::shared_ptr<SdlRendererWrapper> m_renderer;
            std::vector<std::shared_ptr<Component>> m_components;
            bool m_sdl_initialized = false;
        private:
            bool sdlGuard();
    };
}
