#include "inttypes.hpp"
#include "sdl-window.hpp"
#include <iostream>
#include <thread>
#include <memory>
#include <string>
#include <functional>

namespace CzaraEngine {
    
    SdlWindow::SdlWindow(const WindowProperties &properties) :
        Window(properties), sustain(false), m_sdl_window(new SdlWindowWrapper(properties)), 
        m_sdl_renderer(new SdlRendererWrapper(m_sdl_window)),
        m_interface(new DearImGuiInterface(m_sdl_window, m_sdl_renderer)) {
        
        sustainEventLoop();
    }

    SdlWindow::SdlWindow(const SdlWindow &window) : Window(window), sustain(window.sustain),
        m_sdl_window(window.m_sdl_window), m_sdl_renderer(window.m_sdl_renderer), m_interface(window.m_interface) {

        if (m_sdl_window.isNullptr()) {
            throw "SDL Window Creation Failed";
        }
        if (m_sdl_renderer.isNullptr()) {
            throw "SDL Renderer Creation Failed";
        }
        
        sustainEventLoop();
    }

    SdlWindow::SdlWindow(SdlWindow * window) : Window(*window), sustain(window->sustain),
        m_sdl_window(window->m_sdl_window), m_sdl_renderer(window->m_sdl_renderer),
        m_interface(window->m_interface) {
        
        std::cout << "SdlWindow::SdlWindow(WindowProperties *)\n";
        if (m_sdl_window.isNullptr()) {
            throw "SDL Window Creation Failed";
        }
        if (m_sdl_renderer.isNullptr()) {
            throw "SDL Renderer Creation Failed";
        }
        
        sustainEventLoop();
    }

    SdlWindow::~SdlWindow() {
        SDL_Quit();
    }
    bool SdlWindow::addChild(const WindowProperties &properties) {
        try {
            this->children.push_back(Shared<Window>(new SdlWindow(properties)));
        } catch(ui8 errCode) {
            // ToDo: Implement logging for error
            std::cerr << "Error Code, " << errCode << ", was intercepted.\n";
            return false;
        }
        return true;
    }

    void SdlWindow::sustainEventLoop() {
        sustain = true;
        while(isOpen()) {
            SDL_SetRenderDrawColor(m_sdl_renderer->get(), 100, 100, 100, 255);
            SDL_Event sdl_event;
            while(SDL_PollEvent(&sdl_event) > 0) {
                m_interface.get()->processEvent(sdl_event);
                switch (sdl_event.type) {
                    case SDL_WINDOWEVENT:
                        switch(sdl_event.window.event) {
                            case SDL_WINDOWEVENT_CLOSE:
                                sustain = false;
                                break;
                        }
                }
            }
            m_interface.get()->newFrame();
            m_interface.get()->render();
            SDL_RenderClear(m_sdl_renderer->get());
            m_interface.get()->draw();
            SDL_RenderPresent(m_sdl_renderer->get());
        }
    }

    bool SdlWindow::isOpen() {
        return sustain;
    }

    void SdlWindow::addComponent(Shared<SdlComponent> &sdl_component) {
        m_components.push_back(sdl_component);
    }
    Shared<SdlWindowWrapper>& SdlWindow::getWindowWrapper() {
        return m_sdl_window;
    }
    Shared<SdlRendererWrapper>& SdlWindow::getRendererWrapper() {
        return m_sdl_renderer;
    }

    void SdlWindow::setInterface(Shared<DearImGuiInterface> &interface) {
        m_interface = interface;
    }

    ui32 SdlWindow::X_CENTER = SDL_WINDOWPOS_CENTERED;
    ui32 SdlWindow::Y_CENTER = SDL_WINDOWPOS_CENTERED;
}
