#include "sdl.renderer.hpp"

#include "exception.hpp"
#include "SDL.h"

namespace CzaraEngine {
    Sdl2Renderer::Sdl2Renderer(const std::shared_ptr<SDL_Window> &window, const std::string &dev_name):
        Renderer(dev_name), m_window(window),
        m_instance(
            SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE),
            SDL_DestroyRenderer)
    {
        if (!m_instance) {
            THROW_EXCEPTION(EngineExceptionCode::WINDOW_EXCEPTION, SDL_GetError());
        }
        i32 w, h = 0;
        i32 r_w, r_h = 0;
        std::shared_ptr<SDL_Window> win = m_window.lock();
        SDL_GetWindowSize(win.get(), &w, &h);
        SDL_GetRendererOutputSize(m_instance.get(), &r_w, &r_h);
        float scaled_w = static_cast<float>(r_w) / static_cast<float>(w);
        float scaled_h = static_cast<float>(r_h) / static_cast<float>(h);
        SDL_RenderSetScale(m_instance.get(), scaled_w, scaled_h);
    }
    Sdl2Renderer::~Sdl2Renderer() {}
    void Sdl2Renderer::clearRenderBuffer() {
        SDL_RenderClear(m_instance.get());
    }
    void Sdl2Renderer::render() {
        SDL_RenderPresent(m_instance.get());
    }
    void Sdl2Renderer::drawColorRgb(const ui8 &red, const ui8 &green, const ui8 &blue, const ui8 &alpha) {
        SDL_SetRenderDrawColor(m_instance.get(), red, green, blue, alpha);
    }
    std::shared_ptr<SDL_Renderer> Sdl2Renderer::getSdlRenderer() {
        return m_instance;
    }
}