#include "dear-vulkan.interface.hpp"
#include "dear-helper.hpp"
#include "czengine.hpp"
#include <imgui.h>

namespace CzaraEngine {
    DearVulkanInterface::DearVulkanInterface(const std::shared_ptr<SdlVulkanRenderer> &vulkan_renderer, std::shared_ptr<SDL_Window> &window):
        Interface(), m_window(window), m_vulkan_window(), m_vulkan_renderer(vulkan_renderer)
    {
        configureVulkanWindow();
    }
    DearVulkanInterface::~DearVulkanInterface() {}
    void DearVulkanInterface::newFrame() {}
    void DearVulkanInterface::configureVulkanWindow() {
        VkSurfaceKHR surface;
        m_vulkan_window.Surface = surface;
        VkBool32 res;
        i32 width, height;
        std::shared_ptr<SDL_Window> window = m_window.lock();
        SDL_GetWindowSize(window.get(), &width, &height);
        vkGetPhysicalDeviceSurfaceSupportKHR(m_vulkan_renderer->getCurrentGpu(), m_vulkan_renderer->getQueueFamily(), m_vulkan_window.Surface, &res);
        if (res != VK_TRUE) {
            showErrorMessageBox("Vulkan GUI Configuration Error",
                "Error, no WSI Support on the current GPU.\n");
            std::exit(1);
        }
        const VkFormat requestSurfaceImageFormat[] = { VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM };
        const VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
        m_vulkan_window.SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(m_vulkan_renderer->getCurrentGpu(), m_vulkan_window.Surface, requestSurfaceImageFormat, (size_t)IM_ARRAYSIZE(requestSurfaceImageFormat), requestSurfaceColorSpace);
        
        #ifdef APP_UNLIMITED_FRAME_RATE
            VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR };
        #else
            VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_FIFO_KHR };
        #endif
            m_vulkan_window.PresentMode = ImGui_ImplVulkanH_SelectPresentMode(m_vulkan_renderer->getCurrentGpu(), m_vulkan_window.Surface, &present_modes[0], IM_ARRAYSIZE(present_modes));
        //printf("[vulkan] Selected PresentMode = %d\n", wd->PresentMode);

        // Create SwapChain, RenderPass, Framebuffer, etc.
        IM_ASSERT(m_min_image_count >= 2);
        ImGui_ImplVulkanH_CreateOrResizeWindow(
            m_vulkan_renderer->getInstance(), 
            m_vulkan_renderer->getCurrentGpu(),
            m_vulkan_renderer->getDevice(),
            &m_vulkan_window, 
            m_vulkan_renderer->getQueueFamily(), 
            m_vulkan_renderer->getAllocator().get(),
            width, height, 
            m_min_image_count);
    }
}