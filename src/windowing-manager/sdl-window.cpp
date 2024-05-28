#include "inttypes.hpp"
#include "sdl-window.hpp"
#include <iostream>
#include <functional>
#include <memory>
#include <string>

//const std::string window_sustain_thread_id = "SdlWindowSustainThread";

SdlWindow::SdlWindow(const WindowProperties &properties) : Window(properties),
    sustain(false) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize the SDL2 Library\n";
        throw "SDL Instantion failure";
    }
    sdl_window = SDL_CreateWindow(properties.name.c_str(),
        properties.x_window_offset, properties.y_window_offset, properties.width, properties.height, 0);
    if (!sdl_window) {
        throw "SDL Window Creation Failed";
    }
    sdl_window_surface = SDL_GetWindowSurface(sdl_window);
    if (!sdl_window_surface) {
        throw "SDL Window Surface Creation Failed";
    }
    sustainEventLoop();
}

SdlWindow::SdlWindow(const SdlWindow &window) : Window(window), sdl_window(window.sdl_window),
    sdl_window_surface(window.sdl_window_surface), sustain(window.sustain) {
    sdl_window = window.sdl_window;
    sdl_window_surface = window.sdl_window_surface;
    if (!sdl_window) {
        throw "SDL Window Creation Failed";
    }
    if (!sdl_window_surface) {
        throw "SDL Window Surface Creation Failed";
    }
    sustainEventLoop();
}

SdlWindow::SdlWindow(SdlWindow * window) : Window(*window), sdl_window(window->sdl_window),
    sdl_window_surface(window->sdl_window_surface), sustain(window->sustain) {
    sdl_window = window->sdl_window;
    sdl_window_surface = window->sdl_window_surface;
    if (!sdl_window) {
        throw "SDL Window Creation Failed";
    }
    if (!sdl_window_surface) {
        throw "SDL Window Surface Creation Failed";
    }
    sustainEventLoop();
}

SdlWindow::~SdlWindow() {
    if (sdl_window_surface != nullptr) {
        delete sdl_window_surface;
    }
    if (sdl_window != nullptr) {
        delete sdl_window;
    }
}
bool SdlWindow::addChild(const WindowProperties &properties) {
    try {
        this->children.push_back(std::unique_ptr<SdlWindow>(new SdlWindow(properties)));
    } catch(ui8 errCode) {
        // ToDo: Implement logging for error
        std::cerr << "Error Code, " << errCode << ", was intercepted.\n";
        return false;
    }
    return true;
}

std::unique_ptr<Window> SdlWindow::cloneUniquely() {
    return std::make_unique<SdlWindow>(*this);
}

void SdlWindow::sustainEventLoop() {
    sustain = true;
    while(sustain) {
        std::cout << "SdlWindow Sustain: " << sustain << "\n";
        SDL_Event sdl_event;
        while(SDL_PollEvent(&sdl_event) > 0) {
            switch (sdl_event.type) {
                case SDL_QUIT:
                    sustain = false;
                    break;
            }
            SDL_UpdateWindowSurface(sdl_window);
        }
    }
}

void SdlWindow::sustainEventLoopAsync(std::stop_token window_stop_token) {
    sustain = true;
    while(sustain && !window_stop_token.stop_requested()) {
        std::cout << "Sustain: " << sustain << " Stop Requested: " << window_stop_token.stop_requested() << "\n";
        SDL_Event sdl_event;
        while(SDL_PollEvent(&sdl_event) > 0) {
            switch (sdl_event.type) {
                case SDL_QUIT:
                    sustain = false;
                    break;
            }
            SDL_UpdateWindowSurface(sdl_window);
        }
    }
}

bool SdlWindow::isOpen() {
    return sustain;
}

ui32 SdlWindow::X_CENTER = SDL_WINDOWPOS_CENTERED;
ui32 SdlWindow::Y_CENTER = SDL_WINDOWPOS_CENTERED;
