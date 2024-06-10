#pragma once

#define SDL_MAIN_HANDLED
#include "window.hpp"
#include "SDL.h"
#include "shared.hpp"
#include <thread>
#include <vector>
#include "wrapper.hpp"
#include "sdl-wrapper.hpp"
#include "sdl-component.hpp"
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
            virtual void addComponent(Shared<SdlComponent> &sdl_component);
            virtual void setInterface(Shared<DearImGuiInterface> &interface);
            virtual bool addChild(const WindowProperties &properties);
            virtual void sustainEventLoop();
            virtual bool isOpen();
            virtual Shared<SdlWindowWrapper>& getWindowWrapper();
            virtual Shared<SdlRendererWrapper>& getRendererWrapper();
            static ui32 X_CENTER;
            static ui32 Y_CENTER;
        private:
            bool sustain = false;
            Shared<SdlWindowWrapper> m_sdl_window;
            Shared<SdlRendererWrapper> m_sdl_renderer;
            Shared<DearImGuiInterface> m_interface;
            std::vector<Shared<SdlComponent>> m_components;
    };

}
