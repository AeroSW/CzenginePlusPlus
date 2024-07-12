#include "app-logs.hpp"
#include "inttypes.hpp"
#include "sdl-window.hpp"
#include "czengine-config-parser.hpp"
#include "czengine-ux-file-parser.hpp"
#include "event-queue.hpp"

#include <thread>
#include <memory>
#include <string>
#include <functional>
#include <filesystem>

namespace fs = std::filesystem;

namespace CzaraEngine {
    ui32 SdlWindow::X_CENTER = SDL_WINDOWPOS_CENTERED;
    ui32 SdlWindow::Y_CENTER = SDL_WINDOWPOS_CENTERED;
    
    SdlWindow::SdlWindow(const WindowProperties &properties) :
        Window(properties), sustain(false), m_sdl_window(new SdlWindowWrapper(properties)), 
        m_sdl_renderer(new SdlRendererWrapper(m_sdl_window)),
        m_interface(new DearImGuiInterface(m_sdl_window, m_sdl_renderer)) {
        Logger::app_log() << "Spinning up thread to parse Czengine Ux." << endl;
        std::jthread jay(std::bind_front(SdlWindow::processInterface, this));
        Logger::app_log() << "Starting SDL2 Event Loop." << endl;
        sustainEventLoop();
    }

    SdlWindow::SdlWindow(const SdlWindow &window) : Window(window), sustain(window.sustain),
        m_sdl_window(window.m_sdl_window), m_sdl_renderer(window.m_sdl_renderer), 
        m_interface(window.m_interface) {
        
        if (!m_sdl_window) {
            throw "SDL Window Creation Failed";
        }
        if (!m_sdl_renderer) {
            throw "SDL Renderer Creation Failed";
        }
        Logger::app_log() << "Starting SDL2 Event Loop." << endl;
        sustainEventLoop();
    }

    SdlWindow::SdlWindow(SdlWindow * window) : Window(*window), sustain(window->sustain),
        m_sdl_window(window->m_sdl_window), m_sdl_renderer(window->m_sdl_renderer),
        m_interface(window->m_interface) {
        
        if (!m_sdl_window) {
            throw "SDL Window Creation Failed";
        }
        if (!m_sdl_renderer) {
            throw "SDL Renderer Creation Failed";
        }
        Logger::app_log() << "Starting SDL2 Event Loop." << endl;
        sustainEventLoop();
    }

    SdlWindow::~SdlWindow() {
        SDL_Quit();
    }

    bool SdlWindow::addChild(const WindowProperties &properties) {
        try {
            this->children.push_back(std::shared_ptr<Window>(new SdlWindow(properties)));
        } catch(ui8 errCode) {
            // ToDo: Implement logging for error
            Logger::err_log() << "Error Code, " << errCode << ", was intercepted.\n";
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
                m_interface->processEvent(sdl_event);
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
            while (!EventDataQueue::empty()) {
                EventDataObject edo = EventDataQueue::peekType();
                switch (edo) {
                    case EventDataObject::COMPONENT: { // this ("{}" inside a case) is a stupid new feature of C++.
                        std::shared_ptr<Component> component = EventDataQueue::dequeue<Component>();
                        if (!!component) {
                            Logger::app_log() << "Adding a new component to Ux." << endl;
                            m_interface->addComponent(component);
                        }
                        break;
                    } case EventDataObject::COMPONENT_COLLECTION: {
                        std::shared_ptr<std::vector<std::shared_ptr<Component>>> components = EventDataQueue::dequeue<std::vector<std::shared_ptr<Component>>>();
                        if (!!components) {
                            Logger::app_log() << "Multiple new components added to Ux." << endl;
                            m_interface->addComponents(*components);
                        }
                        break;
                    } case EventDataObject::STRING: {
                        //Shared<std::string> component = EventDataQueue::dequeue<std::string>();
                        break;
                    }
                };
            }
            m_interface->newFrame();
            m_interface->drawInterface();

            m_interface->render();
            SDL_RenderClear(m_sdl_renderer->get());
            m_interface->draw();
            SDL_RenderPresent(m_sdl_renderer->get());
        }
    }

    bool SdlWindow::isOpen() {
        return sustain;
    }

    std::shared_ptr<SdlWindowWrapper>& SdlWindow::getWindowWrapper() {
        return m_sdl_window;
    }
    std::shared_ptr<SdlRendererWrapper>& SdlWindow::getRendererWrapper() {
        return m_sdl_renderer;
    }

    void SdlWindow::setInterface(std::shared_ptr<DearImGuiInterface> &interface) {
        m_interface = interface;
    }

    void SdlWindow::processInterface(const std::stop_token &token) {
        CzengineConfigManager config_manager;
        std::string full_path = config_manager.getAppConfig().interface.directory + "/" + config_manager.getAppConfig().interface.name;
        Logger::app_log() << "Starting to parse Ux File, [" << full_path << "]." << endl;
        fs::path interface_file{full_path};
        CzengineUxFileParser ux_file_processor{interface_file};
        std::shared_ptr<void> void_ref {new std::vector<std::shared_ptr<Component>>(ux_file_processor.processFile())};
        EventDataObject edo = EventDataObject::COMPONENT_COLLECTION;
        Logger::app_log() << "Enqueuing parsed Ux Components." << endl;
        EventDataQueue::enqueue(edo, void_ref);
    }

    void SdlWindow::showErrorMessageBox(const std::string &title, const std::string &msg) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title.c_str(), msg.c_str(), NULL);
    }
}
