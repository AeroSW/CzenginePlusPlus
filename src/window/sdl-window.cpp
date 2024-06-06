#include "inttypes.hpp"
#include "sdl-window.hpp"
#include <iostream>
#include <memory>
#include <string>
#include <functional>

namespace CzaraEngine {
    
    SdlWindow::SdlWindow(const WindowProperties &properties, const std::stop_token &stop_token) :
        Window(properties), sustain(false), sdl_window(new SdlWindowWrapper(properties)), 
        sdl_window_surface(new SdlSurfaceWrapper(sdl_window)), m_stop_token(stop_token) {
        sustainEventLoop();
    }

    SdlWindow::SdlWindow(const SdlWindow &window) : Window(window), sustain(window.sustain),
        sdl_window(window.sdl_window), sdl_window_surface(window.sdl_window_surface), m_stop_token(window.m_stop_token) {

        if (sdl_window.isNullptr()) {
            throw "SDL Window Creation Failed";
        }
        if (sdl_window_surface.isNullptr()) {
            throw "SDL Window Surface Creation Failed";
        }
    }

    SdlWindow::SdlWindow(SdlWindow * window) : Window(*window), sustain(window->sustain),
        sdl_window(window->sdl_window), sdl_window_surface(window->sdl_window_surface), m_stop_token(window->m_stop_token) {
        
        std::cout << "SdlWindow::SdlWindow(WindowProperties *)\n";
        if (sdl_window.isNullptr()) {
            throw "SDL Window Creation Failed";
        }
        if (sdl_window_surface.isNullptr()) {
            throw "SDL Window Surface Creation Failed";
        }
    }

    SdlWindow::~SdlWindow() {
        std::cout << "SDL Destructor\n";
    }
    bool SdlWindow::addChild(const WindowProperties &properties) {
        try {
            this->children.push_back(Shared<Window>(new SdlWindow(properties, m_stop_token)));
        } catch(ui8 errCode) {
            // ToDo: Implement logging for error
            std::cerr << "Error Code, " << errCode << ", was intercepted.\n";
            return false;
        }
        return true;
    }

    void SdlWindow::sustainEventLoop() {
        sustain = true;
        std::cout << "Sustain: " << sustain << std::endl;
        while(sustain && !(m_stop_token.stop_requested())) {
            SDL_Event sdl_event;
            while(SDL_PollEvent(&sdl_event) > 0 && sustain) {
                std::cout << "Event detected!\n";
                switch (sdl_event.type) {
                    case SDL_WINDOWEVENT:
                        std::cout << "Window event detected!\n";
                        switch(sdl_event.window.event) {
                            case SDL_WINDOWEVENT_CLOSE:
                                std::cout << "Close event detected!\n";
                                sustain = false;
                                break;
                        }
                }
                if(sustain) SDL_UpdateWindowSurface(sdl_window->get());
            }
        }
    }

    bool SdlWindow::isOpen() {
        return sustain && !m_stop_token.stop_requested();
    }

    ui32 SdlWindow::X_CENTER = SDL_WINDOWPOS_CENTERED;
    ui32 SdlWindow::Y_CENTER = SDL_WINDOWPOS_CENTERED;
}
