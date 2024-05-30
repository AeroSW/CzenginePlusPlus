#pragma once

#define SDL_MAIN_HANDLED
#include "window.hpp"
#include "SDL.h"
#include "shared.hpp"

typedef SDL_Window swindow;
typedef SDL_Surface ssurface;

class SwindowWrapper {
    public:
        SwindowWrapper(const WindowProperties &properties);
        ~SwindowWrapper();
        swindow * get() const;
    private:
        swindow * sdl_window;
};
class SsurfaceWrapper {
    public:
        SsurfaceWrapper(Shared<SwindowWrapper> swindow_wrapper);
        ~SsurfaceWrapper();
        ssurface * get() const;
    private:
        Shared<SwindowWrapper> sdl_window_wrapper;
        ssurface * sdl_surface;
};

class SdlWindow : public Window {
    private:
        bool sustain = false;
        Shared<SwindowWrapper> sdl_window;
        Shared<SsurfaceWrapper> sdl_window_surface;
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
