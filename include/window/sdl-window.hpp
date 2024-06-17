#pragma once

#define SDL_MAIN_HANDLED
#include "window.hpp"
#include "SDL.h"
#include "SDL_events.h"
#include "shared.hpp"
#include <thread>
#include <vector>
#include <queue>
#include "wrapper.hpp"
#include "sdl-wrapper.hpp"
#include "dimgui-interface.hpp"

typedef SDL_Window swindow;
typedef SDL_Surface ssurface;

namespace CzaraEngine {
    class DearImGuiInterface;
    class SdlWindow : public Window {
        public:
            SdlWindow(const WindowProperties &properties);
            SdlWindow(const SdlWindow &window);
            SdlWindow(SdlWindow* window);
            virtual ~SdlWindow();
            virtual void setInterface(Shared<DearImGuiInterface> &interface);
            virtual bool addChild(const WindowProperties &properties);
            virtual void sustainEventLoop();
            virtual bool isOpen();
            virtual Shared<SdlWindowWrapper>& getWindowWrapper();
            virtual Shared<SdlRendererWrapper>& getRendererWrapper();
            static ui32 X_CENTER;
            static ui32 Y_CENTER;
        private: // Variables
            bool sustain = false;
            Shared<SdlWindowWrapper> m_sdl_window;
            Shared<SdlRendererWrapper> m_sdl_renderer;
            Shared<DearImGuiInterface> m_interface;
            SDL_mutex * m_event_mutex;
            std::queue<Shared<void>> data_queue;
        private: // Methods
            void processInterface(const std::stop_token &token);
    };

}
