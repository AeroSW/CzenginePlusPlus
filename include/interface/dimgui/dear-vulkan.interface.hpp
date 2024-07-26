#pragma once

#include "dear.interface.hpp"

#include "backends/imgui_impl_vulkan.h"
#include <sdl-vulkan.renderer.hpp>

namespace CzaraEngine {
    class DearVulkanInterface : public DearInterface {
        public:
            DearVulkanInterface(std::shared_ptr<SDL_Window> &window, const std::shared_ptr<SdlVulkanRenderer> &renderer);
            virtual ~DearVulkanInterface();
            virtual void newFrame();
            virtual void render();
            virtual void draw();
            virtual void drawInterface();
            virtual bool isRendererUninitialized();
            virtual bool isInitialized();
            virtual void initInterfaceRendering();
        private:
            ImGui_ImplVulkanH_Window m_vulkan_window;
            std::weak_ptr<SdlVulkanRenderer> m_vulkan_renderer;
            ui32 m_min_image_count = 2;
            bool m_swap_chain_rebuild = false;
            void configureVulkanWindow();
            void initImGuiBackends();
    };
    static void checkVkResult(VkResult err);
}
