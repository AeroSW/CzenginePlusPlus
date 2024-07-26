#include "dear-vulkan.interface.hpp"
#include "app-logs.hpp"
#include "czengine.hpp"
#include "utility.hpp"
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include "vulkan/vulkan.h"

namespace CzaraEngine {
    DearVulkanInterface::DearVulkanInterface(std::shared_ptr<SDL_Window> &window, const std::shared_ptr<SdlVulkanRenderer> &vulkan_renderer):
        DearInterface(window), m_vulkan_window(), m_vulkan_renderer(vulkan_renderer), m_swap_chain_rebuild(false)
    {
        configureVulkanWindow();
        initImGuiBackends();
    }
    DearVulkanInterface::~DearVulkanInterface() {
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        std::shared_ptr<SdlVulkanRenderer> renderer = m_vulkan_renderer.lock();
        ImGui_ImplVulkanH_DestroyWindow(renderer->getInstance(), renderer->getDevice(), &m_vulkan_window, renderer->getAllocator().get());
    }
    void DearVulkanInterface::newFrame() {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
    }
    void DearVulkanInterface::render() {
        if (sdlGuard(this)) return;
        ImGui::Render();
        ImDrawData* draw_data = ImGui::GetDrawData();
        VkSemaphore image_acquired_semaphore  = m_vulkan_window.FrameSemaphores[m_vulkan_window.SemaphoreIndex].ImageAcquiredSemaphore;
        VkSemaphore render_complete_semaphore = m_vulkan_window.FrameSemaphores[m_vulkan_window.SemaphoreIndex].RenderCompleteSemaphore;
        std::shared_ptr<SdlVulkanRenderer> renderer = m_vulkan_renderer.lock();
        VkResult err = vkAcquireNextImageKHR(renderer->getDevice(), m_vulkan_window.Swapchain, UINT64_MAX, image_acquired_semaphore, VK_NULL_HANDLE, &m_vulkan_window.FrameIndex);
        if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
        {
            m_swap_chain_rebuild = true;
            return;
        }

        ImGui_ImplVulkanH_Frame* fd = &m_vulkan_window.Frames[m_vulkan_window.FrameIndex];
        {
            SdlVulkanRenderer::handleErr(vkWaitForFences(renderer->getDevice(), 1, &fd->Fence, VK_TRUE, UINT64_MAX));
            SdlVulkanRenderer::handleErr(vkResetFences(renderer->getDevice(), 1, &fd->Fence));
        }
        {
            SdlVulkanRenderer::handleErr(vkResetCommandPool(renderer->getDevice(), fd->CommandPool, 0));
            VkCommandBufferBeginInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
            SdlVulkanRenderer::handleErr(vkBeginCommandBuffer(fd->CommandBuffer, &info));
        }
        {
            VkRenderPassBeginInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            info.renderPass = m_vulkan_window.RenderPass;
            info.framebuffer = fd->Framebuffer;
            info.renderArea.extent.width = m_vulkan_window.Width;
            info.renderArea.extent.height = m_vulkan_window.Height;
            info.clearValueCount = 1;
            info.pClearValues = &m_vulkan_window.ClearValue;
            vkCmdBeginRenderPass(fd->CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
        }

        // Record dear imgui primitives into command buffer
        ImGui_ImplVulkan_RenderDrawData(draw_data, fd->CommandBuffer);

        // Submit command buffer
        vkCmdEndRenderPass(fd->CommandBuffer);
        {
            VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            VkSubmitInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            info.waitSemaphoreCount = 1;
            info.pWaitSemaphores = &image_acquired_semaphore;
            info.pWaitDstStageMask = &wait_stage;
            info.commandBufferCount = 1;
            info.pCommandBuffers = &fd->CommandBuffer;
            info.signalSemaphoreCount = 1;
            info.pSignalSemaphores = &render_complete_semaphore;

            SdlVulkanRenderer::handleErr(vkEndCommandBuffer(fd->CommandBuffer));
            SdlVulkanRenderer::handleErr(vkQueueSubmit(renderer->getQueue(), 1, &info, fd->Fence));
        }
    }
    void DearVulkanInterface::draw() {
        // Update and Render additional Platform Windows
        if (m_io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
        if (m_swap_chain_rebuild)
            return;
        VkSemaphore render_complete_semaphore = m_vulkan_window.FrameSemaphores[m_vulkan_window.SemaphoreIndex].RenderCompleteSemaphore;
        VkPresentInfoKHR info = {};
        info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        info.waitSemaphoreCount = 1;
        info.pWaitSemaphores = &render_complete_semaphore;
        info.swapchainCount = 1;
        info.pSwapchains = &m_vulkan_window.Swapchain;
        info.pImageIndices = &m_vulkan_window.FrameIndex;
        std::shared_ptr<SdlVulkanRenderer> renderer = m_vulkan_renderer.lock();
        VkResult err = vkQueuePresentKHR(renderer->getQueue(), &info);
        if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
        {
            m_swap_chain_rebuild = true;
            return;
        }
        SdlVulkanRenderer::handleErr(err);
        m_vulkan_window.SemaphoreIndex = (m_vulkan_window.SemaphoreIndex + 1) % m_vulkan_window.SemaphoreCount;
    }
    void DearVulkanInterface::drawInterface() {
        for (std::shared_ptr<Component> component : m_components) {
            component->renderComponent();
        }
    }
    bool DearVulkanInterface::isRendererUninitialized() {
        return isWeakPtrUnitialized(m_vulkan_renderer);
    }
    bool DearVulkanInterface::isInitialized() {
        return m_sdl_initialized;
    }
    void DearVulkanInterface::initInterfaceRendering() {
        m_sdl_initialized = true;
    }
    void DearVulkanInterface::configureVulkanWindow() {
        VkSurfaceKHR surface;
        std::shared_ptr<SDL_Window> window = m_window.lock();
        std::shared_ptr<SdlVulkanRenderer> renderer = m_vulkan_renderer.lock();
        if (SDL_Vulkan_CreateSurface(window.get(), renderer->getInstance(), &surface) == 0) {
            Logger::err_log() << "Dear Vulkan Interface Error:" << endl;
            Logger::err_log() << "\tFailed to create Vulkan Surface" << endl;
            showErrorMessageBox("Interface Initialization Error", "Failed to create Vulkan Surface.");
            std::exit(1);
        }
        i32 width, height;
        m_vulkan_window.Surface = surface;
        VkBool32 res;
        SDL_GetWindowSize(window.get(), &width, &height);
        vkGetPhysicalDeviceSurfaceSupportKHR(renderer->getCurrentGpu(), renderer->getQueueFamily(), m_vulkan_window.Surface, &res);
        if (res != VK_TRUE) {
            VkPhysicalDeviceProperties props;
            vkGetPhysicalDeviceProperties(renderer->getCurrentGpu(), &props);
            std::string err_msg = "Error, no WSI Support on the current GPU.";
            Logger::err_log() << "Vulkan Interface Error Log:" << endl;
            Logger::err_log() << "\tID: " << props.deviceID << " Device: " << props.deviceName << " Driver Version: " << props.driverVersion << endl;
            Logger::err_log() << "\tType: " << props.deviceType << " Vendor ID: " << props.vendorID << endl;
            Logger::err_log() << "\tError Message: " << err_msg << endl;
            
            showErrorMessageBox("Vulkan GUI Configuration Error",
                err_msg);
            std::exit(1);
        }
        const VkFormat requestSurfaceImageFormat[] = { VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM };
        const VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
        m_vulkan_window.SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(renderer->getCurrentGpu(), m_vulkan_window.Surface, requestSurfaceImageFormat, (size_t)IM_ARRAYSIZE(requestSurfaceImageFormat), requestSurfaceColorSpace);
        
        #ifdef APP_UNLIMITED_FRAME_RATE
            VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR };
        #else
            VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_FIFO_KHR };
        #endif
            m_vulkan_window.PresentMode = ImGui_ImplVulkanH_SelectPresentMode(renderer->getCurrentGpu(), m_vulkan_window.Surface, &present_modes[0], IM_ARRAYSIZE(present_modes));
        //printf("[vulkan] Selected PresentMode = %d\n", m_vulkan_window.PresentMode);

        // Create SwapChain, RenderPass, Framebuffer, etc.
        IM_ASSERT(m_min_image_count >= 2);
        ImGui_ImplVulkanH_CreateOrResizeWindow(
            renderer->getInstance(), 
            renderer->getCurrentGpu(),
            renderer->getDevice(),
            &m_vulkan_window, 
            renderer->getQueueFamily(), 
            renderer->getAllocator().get(),
            width, height, 
            m_min_image_count);
    }
    void DearVulkanInterface::initImGuiBackends() {
        std::shared_ptr<SdlVulkanRenderer> renderer = m_vulkan_renderer.lock();
        std::shared_ptr<SDL_Window> window = m_window.lock();
        ImGui_ImplSDL2_InitForVulkan(window.get());
        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = renderer->getInstance();
        init_info.PhysicalDevice = renderer->getCurrentGpu();
        init_info.Device = renderer->getDevice();
        init_info.QueueFamily = renderer->getQueueFamily();
        init_info.Queue = renderer->getQueue();
        init_info.PipelineCache = renderer->getPipelineCache();
        init_info.DescriptorPool = renderer->getDescriptorPool();
        init_info.RenderPass = m_vulkan_window.RenderPass;
        init_info.Subpass = 0;
        init_info.MinImageCount = m_min_image_count;
        init_info.ImageCount = m_vulkan_window.ImageCount;
        init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        init_info.Allocator = renderer->getAllocator().get();
        init_info.CheckVkResultFn = checkVkResult;
        ImGui_ImplVulkan_Init(&init_info);
    }
    void checkVkResult(VkResult res) {
        if (res == 0) return;
        Logger::err_log() << "Vulkan Error: VkResult: " << res << endl;
        if (res < 0) {
            std::exit(1);
        }
    }
}