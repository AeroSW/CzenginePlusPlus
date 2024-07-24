#pragma once

#include "inttypes.hpp"
#include "renderer.hpp"
#include "sdl-renderer-binding-macro.hpp"

#include "SDL.h"

#include <memory>
#include <string>

namespace CzaraEngine {
    class Sdl2Renderer : public Renderer {
        public:
            Sdl2Renderer(const std::shared_ptr<SDL_Window> &window, const std::string &device = "");
            virtual ~Sdl2Renderer();
            virtual void drawColorRgb(const ui8 &red, const ui8 &green, const ui8 &blue, const ui8 &alpha);
            virtual void clearRenderBuffer();
            virtual void render();
        private:
            std::weak_ptr<SDL_Window> m_window;
            std::shared_ptr<SDL_Renderer> m_instance;
    };
}

BIND_RENDERER(SDL2, CzaraEngine::Sdl2Renderer, const std::shared_ptr<SDL_Window>&, const std::string&);
