#pragma once

#define SDL_MAIN_HANDLED
#include "window.hpp"
#include "SDL.h"

class SdlWindow : public Window {
    private:
        SDL_Window * sdl_window;
        SDL_Surface * sdl_window_surface;
        bool sustain = false;
    public:
        SdlWindow(const WindowProperties &properties);
        SdlWindow(const SdlWindow &window);
        SdlWindow(SdlWindow* window);
        virtual ~SdlWindow();
        virtual bool addChild(const WindowProperties &properties);
        virtual std::unique_ptr<Window> cloneUniquely();
        virtual void sustainEventLoop();
        virtual bool isOpen();
        static ui32 X_CENTER;
        static ui32 Y_CENTER;
};
