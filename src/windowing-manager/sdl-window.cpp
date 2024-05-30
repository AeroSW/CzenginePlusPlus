#include "inttypes.hpp"
#include "sdl-window.hpp"
#include <iostream>
#include <memory>
#include <string>

SwindowWrapper::SwindowWrapper(const WindowProperties &properties) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize the SDL2 Library\n";
        throw "SDL Instantion failure";
    }
    sdl_window = SDL_CreateWindow(properties.name.c_str(),
       properties.x_window_offset, properties.y_window_offset, properties.width, properties.height, 0);
}
SwindowWrapper::~SwindowWrapper() {
    //if (sdl_window != nullptr) delete sdl_window;
    SDL_DestroyWindow(sdl_window);
}
swindow * SwindowWrapper::get() const {
    return sdl_window;
}

SsurfaceWrapper::SsurfaceWrapper(Shared<SwindowWrapper> swindow_wrapper) : sdl_window_wrapper(swindow_wrapper) {
    sdl_surface = SDL_GetWindowSurface(swindow_wrapper->get());
    if (!sdl_surface) {
        throw "SDL Window Surface Creation Failed";
    }
}
SsurfaceWrapper::~SsurfaceWrapper() {
    if (sdl_surface != nullptr) delete sdl_surface;
    SDL_DestroyWindowSurface(sdl_window_wrapper->get());
}
ssurface * SsurfaceWrapper::get() const {
    return sdl_surface;
}

SdlWindow::SdlWindow(const WindowProperties &properties) : Window(properties),
    sustain(false), sdl_window(new SwindowWrapper(properties)), sdl_window_surface(new SsurfaceWrapper(sdl_window.get())) {
    sustainEventLoop();
}

SdlWindow::SdlWindow(const SdlWindow &window) : Window(window), sustain(window.sustain),
    sdl_window(window.sdl_window), sdl_window_surface(window.sdl_window_surface) {
    if (!sdl_window) {
        throw "SDL Window Creation Failed";
    }
    if (!sdl_window_surface) {
        throw "SDL Window Surface Creation Failed";
    }
    sustainEventLoop();
}

SdlWindow::SdlWindow(SdlWindow * window) : Window(*window), sustain(window->sustain),
    sdl_window(window->sdl_window), sdl_window_surface(window->sdl_window_surface) {
    if (!sdl_window) {
        throw "SDL Window Creation Failed";
    }
    if (!sdl_window_surface) {
        throw "SDL Window Surface Creation Failed";
    }
    sustainEventLoop();
}

SdlWindow::~SdlWindow() {}
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
        SDL_Event sdl_event;
        while(SDL_PollEvent(&sdl_event) > 0) {
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
            SDL_UpdateWindowSurface(sdl_window->get());
        }
    }
}

bool SdlWindow::isOpen() {
    return sustain;
}

ui32 SdlWindow::X_CENTER = SDL_WINDOWPOS_CENTERED;
ui32 SdlWindow::Y_CENTER = SDL_WINDOWPOS_CENTERED;
