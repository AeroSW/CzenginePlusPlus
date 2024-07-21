#include "sdl.renderer.hpp"

namespace CzaraEngine {
    Sdl2Renderer::Sdl2Renderer(const std::string &dev_name, const std::shared_ptr<SDL_Window> &window):
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
    std::shared_ptr<DearImGuiInterface> Sdl2Renderer::getInterface() {
        return std::make_shared<DearImGuiInterface>()
    }
}