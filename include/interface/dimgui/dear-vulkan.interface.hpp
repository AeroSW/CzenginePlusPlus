#pragma once

#include "dear-sdl.interface.hpp"

#include "backends/imgui_impl_vulkan.h"
#include <sdl-vulkan.renderer.hpp>

namespace CzaraEngine {
    class DearVulkanInterface : public Interface {
        public:
            DearVulkanInterface(const std::shared_ptr<SdlVulkanRenderer> &renderer, std::shared_ptr<SDL_Window> &window);
            virtual ~DearVulkanInterface();
            virtual void newFrame() override;
            friend bool vulkanSdlGuard(DearImGuiInterface &interface);
        private:
            ImGui_ImplVulkanH_Window m_vulkan_window;
            std::shared_ptr<SdlVulkanRenderer> m_vulkan_renderer;
            std::weak_ptr<SDL_Window> m_window;
            ui32 m_min_image_count = 2;
            void configureVulkanWindow();
    };
}
