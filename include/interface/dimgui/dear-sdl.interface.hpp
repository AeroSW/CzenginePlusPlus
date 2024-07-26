#pragma once

#include "dear.interface.hpp"
#include "sdl.renderer.hpp"

#include "backends/imgui_impl_sdlrenderer2.h"

#include <vector>

namespace CzaraEngine {
    class DearSdlInterface : public DearInterface {
        public:
            DearSdlInterface(const std::shared_ptr<SDL_Window> &, const std::shared_ptr<Sdl2Renderer> &);
            virtual ~DearSdlInterface();
            virtual void newFrame();
            virtual void render();
            virtual void draw();
            virtual void drawInterface();
            virtual bool isRendererUninitialized();
            virtual bool isInitialized();
            virtual void initInterfaceRendering();
        protected:
            std::weak_ptr<Sdl2Renderer> m_renderer;
    };
}
