#pragma once

#define SDL_MAIN_HANDLED
#include "window.hpp"
#include "SDL.h"
#include "shared.hpp"
#include <thread>
#include "wrapper.hpp"
#include "sdl-wrapper.hpp"

typedef SDL_Window swindow;
typedef SDL_Surface ssurface;

namespace CzaraEngine {
    typedef std::pair<SDL_Surface*, Shared<Wrapper<SDL_Window>>> SdlSurfacePair;
    void destroySdlWindow();
    void destroySdlSurface();
    void destroySdlRenderer();
    class SdlWindow : public Window {
        public:
            SdlWindow(const WindowProperties &properties, const std::stop_token &stop_token);
            SdlWindow(const SdlWindow &window);
            SdlWindow(SdlWindow* window);
            virtual ~SdlWindow();
            virtual bool addChild(const WindowProperties &properties);
            virtual void sustainEventLoop();
            virtual bool isOpen();
            static ui32 X_CENTER;
            static ui32 Y_CENTER;
        private:
            bool sustain = false;
            std::stop_token m_stop_token;
            Shared<SdlWindowWrapper> sdl_window;
            Shared<SdlSurfaceWrapper> sdl_window_surface;
            Shared<SdlRendererWrapper> sdl_renderer;
    };

}
