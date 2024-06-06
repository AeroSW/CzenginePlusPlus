#pragma once

#include "SDL.h"
#include "shared.hpp"

namespace CzaraEngine {
    class SdlComponent {
        public:
            virtual ~SdlComponent() {}
            virtual void drawComponent() = 0;
            virtual void destroyComponent() = 0;
            virtual void hasFocus() = 0;
            virtual void inputListener(SDL_Event &sdl_event) = 0;
        protected:
            Shared<SDL_Renderer> m_renderer;
    };
}
