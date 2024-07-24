#pragma once

#include "renderer.hpp"
#include "sdl-renderer-binding-macro.hpp"

#include <SDL.h>
#include <SDL_vulkan.h>
#include "vulkan/vulkan.h"

#include <memory>
#include <string>
#include <vector>

namespace CzaraEngine {
    class SdlVulkanRenderer : public Renderer {
        public:
            SdlVulkanRenderer(const std::shared_ptr<SDL_Window> &window, const std::string &dev_name = "");
            ~SdlVulkanRenderer();
            void handleErr(const VkResult &err);
            bool isExtensionAvailable(const std::vector<VkExtensionProperties> &ext_props, const std::string &extension);
            void cleanup();
            static SDL_WindowFlags getFlag();

            const std::unique_ptr<VkAllocationCallbacks> getAllocator();
            const VkInstance& getInstance();
            const VkDevice& getDevice();
            const VkPhysicalDevice& getCurrentGpu();
            const VkQueue& getQueue();
            const ui32& getQueueFamily();

            virtual void render();
            virtual void clearRenderBuffer();
            virtual void drawColorRgb(const ui8 &red, const ui8 &green, const ui8 &blue, const ui8 &alpha);

        private:
            std::weak_ptr<SDL_Window> m_window;

            std::unique_ptr<VkAllocationCallbacks> m_allocator = nullptr;
            std::vector<const char*> m_extensions;
            VkInstance m_instance = VK_NULL_HANDLE;
            std::vector<VkPhysicalDevice> m_gpus;
            VkPhysicalDevice m_curr_gpu = VK_NULL_HANDLE;
            VkDevice m_device = VK_NULL_HANDLE;
            VkQueue m_queue = VK_NULL_HANDLE;
            VkDebugReportCallbackEXT m_dubug_report = VK_NULL_HANDLE;
            VkPipelineCache m_pipeline_cache = VK_NULL_HANDLE;
            VkDescriptorPool m_descriptor_pool = VK_NULL_HANDLE;
            ui32 m_queue_family = (ui32) - 1;

        private:
            void setup();
            void setupInstance();
            void setupLogicalDevice();
            void setupDescriptorPool();
            void selectPhysicalDevice();
            void selectGraphicsQueueFamily();
    };
}

BIND_RENDERER(Vulkan, CzaraEngine::SdlVulkanRenderer, const std::shared_ptr<SDL_Window>&, const std::string&);
