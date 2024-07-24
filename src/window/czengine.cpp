#include "czengine.hpp"

#include "app-logs.hpp"
#include "inttypes.hpp"
#include "event-queue.hpp"
#include "sdl-renderer-binding-factory.hpp"

#include <thread>
#include <memory>
#include <string>
#include <functional>
#include <filesystem>

namespace fs = std::filesystem;

namespace CzaraEngine {
    
    Czengine::Czengine(const CzengineAppConfig &config) : sustain(false),
        m_window(createWindow(config)), m_renderer(createRenderer(config, m_window))
    {}

    Czengine::Czengine(const Czengine &window) : sustain(window.sustain),
        m_window(window.m_window), m_renderer(window.m_renderer), 
        m_interfaces(window.m_interfaces) {
        
        if (!m_window) {
            throw "SDL Window Creation Failed";
        }
        if (!m_renderer) {
            throw "SDL Renderer Creation Failed";
        }
        Logger::app_log() << "Starting SDL2 Event Loop." << endl;
        sustainEventLoop();
    }

    Czengine::Czengine(Czengine * window) : sustain(window->sustain),
        m_window(window->m_window), m_renderer(window->m_renderer),
        m_interfaces(window->m_interfaces) {
        
        if (!m_window) {
            throw "SDL Window Creation Failed";
        }
        if (!m_renderer) {
            throw "SDL Renderer Creation Failed";
        }
        Logger::app_log() << "Starting SDL2 Event Loop." << endl;
        sustainEventLoop();
    }

    Czengine::~Czengine() {
        SDL_Quit();
    }

    void Czengine::sustainEventLoop() {
        sustain = true;
        while(isOpen()) {
            m_renderer->drawColorRgb(100, 100, 100, 255);
            SDL_Event sdl_event;
            while(SDL_PollEvent(&sdl_event) > 0) {
                for (std::shared_ptr<Interface> &interface : m_interfaces) {
                    interface->processEvent(sdl_event);
                }
                switch (sdl_event.type) {
                    case SDL_WINDOWEVENT:
                        switch(sdl_event.window.event) {
                            case SDL_WINDOWEVENT_CLOSE:
                                Logger::app_log() << "Window close event detected.  Closing SDL2 Window..." << endl;
                                sustain = false;
                                break;
                        }
                }
            }
            m_renderer->clearRenderBuffer();
            for (std::shared_ptr<Interface> &interface : m_interfaces) {
                interface->newFrame();
                interface->drawInterface();
                interface->render();
                interface->draw();
            }
            // Based on Documentation, SDL uses a back renderer. (Renders at the end of its workflow.)
            m_renderer->render();
        }
    }

    bool Czengine::isOpen() {
        return sustain;
    }

    void Czengine::addInterface(const std::shared_ptr<Interface> &interface) {
        m_interfaces.push_back(interface);
    }

    void showErrorMessageBox(const std::string &title, const std::string &msg) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title.c_str(), msg.c_str(), NULL);
    }

    std::shared_ptr<SDL_Window> createWindow(const CzengineAppConfig &config) {
        std::shared_ptr<SDL_Window> window{
            SDL_CreateWindow(
                config.title.c_str(), 
                config.window.x_window_offset, 
                config.window.y_window_offset, 
                config.window.width, 
                config.window.height, 0),
            SDL_DestroyWindow
        };
        return window;
    }
    std::shared_ptr<Renderer> createRenderer(const CzengineAppConfig &config, const std::weak_ptr<SDL_Window> &wwindow) {
        std::shared_ptr<SDL_Window> swindow = wwindow.lock();
        std::shared_ptr<Renderer> renderer{
            SdlRendererBindingFactory::createRenderer(config.render.api, swindow, config.render.device)
        };
        return renderer;
    }
    ui32& getCenterXPos() {
        static ui32 GLOBAL_X_CENTER = SDL_WINDOWPOS_CENTERED;
        return GLOBAL_X_CENTER;
    }
    ui32& getCenterYPos() {
        static ui32 GLOBAL_Y_CENTER = SDL_WINDOWPOS_CENTERED;
        return GLOBAL_Y_CENTER;
    }
}
