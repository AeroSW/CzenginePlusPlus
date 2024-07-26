#pragma once

#define SDL_MAIN_HANDLED

// Local Includes
#include "czengine-app-config.hpp"
#include "interface.hpp"
#include "renderer.hpp"

// Extern Libs
#include "SDL.h"
#include "SDL_events.h"

// Standard C++ Libs
#include <memory>
#include <queue>
#include <vector>

namespace CzaraEngine {
    struct CzengineAppConfig;
    class Czengine {
        public:
            Czengine(const CzengineAppConfig &config);
            Czengine(const Czengine &window);
            Czengine(Czengine* window);
            virtual ~Czengine();
            virtual void addInterface(const std::shared_ptr<CzengineInterface> &interface);
            virtual std::shared_ptr<SDL_Window> getWindow();
            virtual std::shared_ptr<Renderer> getRenderer();
            virtual void sustainEventLoop();
            virtual bool isOpen();

        private: // Variables
            bool sustain = false;
            std::shared_ptr<SDL_Window> m_window;
            std::shared_ptr<Renderer> m_renderer;
            std::vector<std::shared_ptr<CzengineInterface>> m_interfaces;
    };
    std::shared_ptr<SDL_Window> createWindow(const CzengineAppConfig &config);
    std::shared_ptr<Renderer> createRenderer(const CzengineAppConfig &config, const std::weak_ptr<SDL_Window> &window);
    void showErrorMessageBox(const std::string &title, const std::string &msg);
    ui32& getCenterXPos();
    ui32& getCenterYPos();
}
