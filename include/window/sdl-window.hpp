#pragma once

#define SDL_MAIN_HANDLED
#include "window.hpp"
#include "SDL.h"
#include "SDL_events.h"
#include <memory>
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
            virtual void setInterface(std::shared_ptr<DearImGuiInterface> &interface);
            virtual bool addChild(const WindowProperties &properties);
            virtual void sustainEventLoop();
            virtual bool isOpen();
            virtual std::shared_ptr<SdlWindowWrapper>& getWindowWrapper();
            virtual std::shared_ptr<SdlRendererWrapper>& getRendererWrapper();
            static ui32 X_CENTER;
            static ui32 Y_CENTER;

            static void showErrorMessageBox(const std::string &title, const std::string &msg);
        private: // Variables
            bool sustain = false;
            std::shared_ptr<SdlWindowWrapper> m_sdl_window;
            std::shared_ptr<SdlRendererWrapper> m_sdl_renderer;
            std::shared_ptr<DearImGuiInterface> m_interface;
            SDL_mutex * m_event_mutex;
            std::queue<std::shared_ptr<void>> data_queue;
            
        private: // Methods
            void processInterface(const std::stop_token &token);
    };

}
