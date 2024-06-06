#pragma once

#include "sdl-component.hpp"

namespace CzaraEngine {
    class SdlMenu : public SdlComponent {
        SdlMenu();
        virtual ~SdlMenu();
        virtual void drawComponent();
        virtual void destroyComponent();
        virtual void hasFocus();
        virtual void inputListener(SDL_Event &sdl_event);
    };
}