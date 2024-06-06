#include "inttypes.hpp"
#include "sdl-window.hpp"
#include <iostream>
#include <memory>
#include <string>

namespace CzaraEngine {
    SwindowWrapper::SwindowWrapper(const WindowProperties &properties) {
        std::cout << "SwindowWrapper::SwindowWrapper(const WindowProperties&)\n";
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "Failed to initialize the SDL2 Library\n";
            throw "SDL Instantion failure";
        }
        sdl_window = SDL_CreateWindow(properties.name.c_str(),
        properties.x_window_offset, properties.y_window_offset, properties.width, properties.height, 0);
    }
    SwindowWrapper::~SwindowWrapper() {
        std::cout << "SwindowWrapper Destructor\n";
        SDL_DestroyWindow(sdl_window);
    }
    swindow * SwindowWrapper::get() const {
        std::cout << "SwindowWrapper::get()\n";
        return sdl_window;
    }

    SsurfaceWrapper::SsurfaceWrapper(const Shared<SwindowWrapper> &swindow_wrapper) : sdl_window_wrapper(swindow_wrapper) {
        std::cout << "SsurfaceWrapper::SsurfaceWrapper(const Shared<SwindowWrapper>&)\n";
        sdl_surface = SDL_GetWindowSurface(sdl_window_wrapper->get());
        if (!sdl_surface) {
            throw "SDL Window Surface Creation Failed";
        }
    }
    SsurfaceWrapper::~SsurfaceWrapper() {
        std::cout << "SsurfaceWrapper Destructor\n";
        SDL_DestroyWindowSurface(sdl_window_wrapper->get());
    }
    ssurface * SsurfaceWrapper::get() const {
        std::cout << "SsurfaceWrapper::get()\n";
        return sdl_surface;
    }

    SdlWindow::SdlWindow(const WindowProperties &properties) : Window(properties),
        sustain(false), sdl_window(new SwindowWrapper(properties)), sdl_window_surface(new SsurfaceWrapper(sdl_window.get())) {
        std::cout << "SdlWindow::SdlWindow(const WindowProperties &)\n";
        std::cout << "Starting loop sustain.\n";
        sustainEventLoop();
        std::cout << "Ending loop sustain\n";
    }

    SdlWindow::SdlWindow(const SdlWindow &window) : Window(window), sustain(window.sustain),
        sdl_window(window.sdl_window), sdl_window_surface(window.sdl_window_surface) {
        std::cout << "SdlWindow::SdlWindow(const SdlWindow &)\n";
        if (sdl_window.isNullptr()) {
            throw "SDL Window Creation Failed";
        }
        if (sdl_window_surface.isNullptr()) {
            throw "SDL Window Surface Creation Failed";
        }
        sustainEventLoop();
    }

    SdlWindow::SdlWindow(SdlWindow * window) : Window(*window), sustain(window->sustain),
        sdl_window(window->sdl_window), sdl_window_surface(window->sdl_window_surface) {
        std::cout << "SdlWindow::SdlWindow(WindowProperties *)\n";
        if (sdl_window.isNullptr()) {
            throw "SDL Window Creation Failed";
        }
        if (sdl_window_surface.isNullptr()) {
            throw "SDL Window Surface Creation Failed";
        }
        sustainEventLoop();
    }

    SdlWindow::~SdlWindow() {
        std::cout << "SDL Destructor\n";
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
        while(sustain) {
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
        return sustain;
    }

    ui32 SdlWindow::X_CENTER = SDL_WINDOWPOS_CENTERED;
    ui32 SdlWindow::Y_CENTER = SDL_WINDOWPOS_CENTERED;
}
