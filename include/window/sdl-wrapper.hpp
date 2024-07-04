#pragma once

#include "window.hpp"
#include "wrapper.hpp"
#include "SDL.h"

namespace CzaraEngine {

    class SdlWindowWrapper : public Wrapper<SDL_Window> {
        public:
            SdlWindowWrapper(const WindowProperties &properties);
            virtual ~SdlWindowWrapper();
            void destroyWindow(SDL_Window *);
    };
    class SdlSurfaceWrapper : public Wrapper<SDL_Surface> {
        public:
            SdlSurfaceWrapper(const std::shared_ptr<SdlWindowWrapper> &swindow_wrapper);
            virtual ~SdlSurfaceWrapper();
            void destroySurface(SDL_Surface *);
        private:
            std::shared_ptr<SdlWindowWrapper> sdl_window_wrapper;
    };
    class SdlRendererWrapper : public Wrapper<SDL_Renderer> {
        public:
            SdlRendererWrapper(const std::shared_ptr<SdlWindowWrapper> &window_wrapper);
            virtual ~SdlRendererWrapper();
            void destroyRenderer(SDL_Renderer *);
        private:
            std::shared_ptr<SdlWindowWrapper> sdl_window_wrapper;

    };
}
