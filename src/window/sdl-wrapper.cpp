#include "sdl-wrapper.hpp"
#include <functional>
#include <iostream>

namespace CzaraEngine {
    SdlWindowWrapper::SdlWindowWrapper(const WindowProperties &properties) :
        Wrapper<SDL_Window>(std::bind_front(SdlWindowWrapper::destroyWindow, this)) {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "Failed to initialize the SDL2 Library\n";
            throw "SDL Instantion failure";
        }
        m_instance = SDL_CreateWindow(properties.name.c_str(),
        properties.x_window_offset, properties.y_window_offset, properties.width, properties.height, 0);
    }
    SdlWindowWrapper::~SdlWindowWrapper() {
    }
    void SdlWindowWrapper::destroyWindow(SDL_Window * window) {
        SDL_DestroyWindow(m_instance);
    }

    SdlSurfaceWrapper::SdlSurfaceWrapper(const std::shared_ptr<SdlWindowWrapper> &swindow_wrapper) : 
        Wrapper<SDL_Surface>(std::bind_front(SdlSurfaceWrapper::destroySurface, this)), sdl_window_wrapper(swindow_wrapper) {
    
        m_instance = SDL_GetWindowSurface(sdl_window_wrapper->get());
        if (!m_instance) {
            throw "SDL Window Surface Creation Failed";
        }
    }
    SdlSurfaceWrapper::~SdlSurfaceWrapper() {
    }
    void SdlSurfaceWrapper::destroySurface(SDL_Surface * unused) {
        SDL_DestroyWindowSurface(sdl_window_wrapper->get());
    }

    SdlRendererWrapper::SdlRendererWrapper(const std::shared_ptr<SdlWindowWrapper> &window_wrapper) :
        Wrapper<SDL_Renderer>(std::bind_front(SdlRendererWrapper::destroyRenderer, this)), sdl_window_wrapper(window_wrapper) {
        
        m_instance = SDL_CreateRenderer(sdl_window_wrapper->get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
        if (!m_instance) {
            THROW_EXCEPTION(EngineExceptionCode::WINDOW_EXCEPTION, SDL_GetError());
        }
        i32 w, h = 0;
        i32 r_w, r_h = 0;
        SDL_GetWindowSize(sdl_window_wrapper.get()->get(), &w, &h);
        SDL_GetRendererOutputSize(m_instance, &r_w, &r_h);
        float scaled_w = static_cast<float>(r_w) / static_cast<float>(w);
        float scaled_h = static_cast<float>(r_h) / static_cast<float>(h);
        SDL_RenderSetScale(m_instance, scaled_w, scaled_h);
    }
    SdlRendererWrapper::~SdlRendererWrapper() {}
    void SdlRendererWrapper::destroyRenderer(SDL_Renderer * renderer) {
        SDL_DestroyRenderer(m_instance);
    }
}