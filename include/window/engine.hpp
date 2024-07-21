#pragma once

#define SDL_MAIN_HANDLED

// Local Includes
#include "dear-sdl.interface.hpp"
#include "renderer.hpp"
#include "window.hpp"
#include "wrapper.hpp"

// Extern Libs
#include "SDL.h"
#include "SDL_events.h"

// Standard C++ Libs
#include <memory>
#include <queue>
#include <thread>
#include <vector>

typedef SDL_Window swindow;
typedef SDL_Surface ssurface;

namespace CzaraEngine {
    struct WindowProperties {
        ui32 width;
        ui32 height;
        ui32 x_window_offset;
        ui32 y_window_offset;
        std::string name;
        std::string renderer;
        std::string render_device;
    };

    class Czengine {
        public:
            Czengine(const WindowProperties &properties);
            Czengine(const Czengine &window);
            Czengine(Czengine* window);
            virtual ~Czengine();
            virtual void setInterface(std::shared_ptr<Interface> &interface);
            virtual bool addChild(const WindowProperties &properties);
            virtual void sustainEventLoop();
            virtual bool isOpen();
            static ui32 X_CENTER;
            static ui32 Y_CENTER;

            static void showErrorMessageBox(const std::string &title, const std::string &msg);

        private: // Variables
            bool sustain = false;
            std::shared_ptr<SDL_Window> m_window;
            std::shared_ptr<Renderer> m_renderer;
            std::shared_ptr<Interface> m_interface;
            SDL_mutex * m_event_mutex;
            std::queue<std::shared_ptr<void>> data_queue;
            
        private: // Methods
            void processInterface(const std::stop_token &token);
    };

}
