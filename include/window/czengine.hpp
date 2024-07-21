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
            virtual void addInterface(const Interface &interface);
            virtual void sustainEventLoop();
            virtual bool isOpen();

        private: // Variables
            bool sustain = false;
            std::shared_ptr<SDL_Window> m_window;
            std::shared_ptr<Renderer> m_renderer;
            std::vector<Interface> m_interfaces;
    };
    std::shared_ptr<SDL_Window>& createWindow(const CzengineAppConfig &config);
    std::shared_ptr<Renderer>& createRenderer(const CzengineAppConfig &config);
    void showErrorMessageBox(const std::string &title, const std::string &msg);
    const ui32 GLOBAL_X_CENTER = SDL_WINDOWPOS_CENTERED;
    const ui32 GLOBAL_Y_CENTER = SDL_WINDOWPOS_CENTERED;
}
