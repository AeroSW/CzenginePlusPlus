#include "app-configuration.hpp"
#include "inttypes.hpp"
#include "sdl-window.hpp"
#include "czengine-ux-file-parser.hpp"
#include "event-queue.hpp"

#include "imgui.h"

#include <iostream>
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
        std::jthread jay(std::bind_front(SdlWindow::processInterface, this));
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
            while (!EventDataQueue::empty()) {
                EventDataObject edo = EventDataQueue::peekType();
                switch (edo) {
                    case EventDataObject::COMPONENT: { // this ("{}" inside a case) is a stupid new feature of C++.
                        std::shared_ptr<Component> component = EventDataQueue::dequeue<Component>();
                        if (!!component) {
                            m_interface.get()->addComponent(component);
                        }
                        break;
                    } case EventDataObject::COMPONENT_COLLECTION: {
                        std::shared_ptr<std::vector<std::shared_ptr<Component>>> components = EventDataQueue::dequeue<std::vector<std::shared_ptr<Component>>>();
                        if (!!components) {
                            m_interface.get()->addComponents(*(components.get()));
                        }
                        break;
                    } case EventDataObject::STRING: {
                        //Shared<std::string> component = EventDataQueue::dequeue<std::string>();
                        break;
                    }
                };
            }
            m_interface.get()->newFrame();
            m_interface.get()->drawInterface();

            // ImGui::DockSpaceOverViewport();
            // ImGui::Begin("Init Panel");
            // ImGui::Text("Panel Content");
            // ImGui::End();

            m_interface.get()->render();
            SDL_RenderClear(m_sdl_renderer->get());
            m_interface.get()->draw();
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
        fs::path interface_file{app_config.getReference().default_interface_file};
        CzengineUxFileParser ux_file_processor{interface_file};
        std::shared_ptr<void> void_ref {new std::vector<std::shared_ptr<Component>>(ux_file_processor.processFile())};
        EventDataObject edo = EventDataObject::COMPONENT_COLLECTION;
        EventDataQueue::enqueue(edo, void_ref);
    }

}
