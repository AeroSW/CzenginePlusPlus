#include "sdl-vulkan.renderer.hpp"
#include "app-logs.hpp"

#include <assert.h>

namespace CzaraEngine {
    // A lot of this code is transcribed from Dear ImGui's SDL2 + Vulkan example.
    SdlVulkanRenderer::SdlVulkanRenderer(const std::shared_ptr<SDL_Window> &window, const std::string &dev_name) :
        Renderer(dev_name), m_window(window), m_extensions(), m_allocator() {
        
        ui32 ext_count = 0;
        SDL_Vulkan_GetInstanceExtensions(window.get(), &ext_count, nullptr);
        m_extensions.resize(ext_count);
        SDL_Vulkan_GetInstanceExtensions(window.get(), &ext_count, m_extensions.data());
        setup();
    }
    SdlVulkanRenderer::~SdlVulkanRenderer() {
        cleanup();
    }
    void SdlVulkanRenderer::handleErr(const VkResult &err) {
        if (err == 0) return;
        Logger::err_log() << "Vulkan Error: VkResult: " << err << endl;
        if (err < 0) {
            std::exit(1);
        }
    }
    SDL_WindowFlags SdlVulkanRenderer::getFlag() {
        return SDL_WINDOW_VULKAN;
    }

    void SdlVulkanRenderer::render() {}
    void SdlVulkanRenderer::clearRenderBuffer() {}
    void SdlVulkanRenderer::drawColorRgb(const ui8 &red, const ui8 &green, const ui8 &blue, const ui8 &alpha) {}

    bool SdlVulkanRenderer::isExtensionAvailable(const std::vector<VkExtensionProperties> &ext_props, const std::string &extension) {
        for (const VkExtensionProperties& props : ext_props) {
            if (strcmp(props.extensionName, extension.c_str())) {
                return true;
            }
        }
        return false;
    }
    void SdlVulkanRenderer::setup() {
        setupInstance();
        selectPhysicalDevice();
        selectGraphicsQueueFamily();
        setupLogicalDevice();
        setupDescriptorPool();
    }
    void SdlVulkanRenderer::setupInstance() {
        VkInstanceCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        ui32 prop_count;
        vkEnumerateInstanceExtensionProperties(nullptr, &prop_count, nullptr);
        std::vector<VkExtensionProperties> ext_props{prop_count};
        handleErr(vkEnumerateInstanceExtensionProperties(nullptr, &prop_count, ext_props.data()));

        if (isExtensionAvailable(ext_props, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME)) {
            m_extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
        }
        #ifdef VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME
            if (isExtensionAvailable(ext_props, VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME)) {
                m_extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
                create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
            }
        #endif
        create_info.enabledExtensionCount = (ui32) m_extensions.size();
        create_info.ppEnabledExtensionNames = m_extensions.data();
        handleErr(vkCreateInstance(&create_info, m_allocator.get(), &m_instance));
    }
    void SdlVulkanRenderer::setupLogicalDevice() {
        std::vector<const char*> device_extensions;
        device_extensions.push_back("VK_KHR_swapchain");
        ui32 pcounter;
        vkEnumerateDeviceExtensionProperties(m_curr_gpu, nullptr, &pcounter, nullptr);
        std::vector<VkExtensionProperties> props{pcounter};
        vkEnumerateDeviceExtensionProperties(m_curr_gpu, nullptr, &pcounter, props.data());
        const float q_priority[] = { 1.0f };
        VkDeviceQueueCreateInfo queue_info[1] = {};
        queue_info[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_info[0].queueFamilyIndex = m_queue_family;
        queue_info[0].queueCount = 1;
        queue_info[0].pQueuePriorities = q_priority;
        VkDeviceCreateInfo create_info = {};
        create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        create_info.queueCreateInfoCount = (sizeof(queue_info) / sizeof(queue_info[0]));
        create_info.pQueueCreateInfos = queue_info;
        create_info.enabledExtensionCount = (ui32) device_extensions.size();
        create_info.ppEnabledExtensionNames = device_extensions.data();
        handleErr(vkCreateDevice(m_curr_gpu, &create_info, m_allocator.get(), &m_device));
        vkGetDeviceQueue(m_device, m_queue_family, 0, &m_queue);
    }
    void SdlVulkanRenderer::setupDescriptorPool() {
        VkDescriptorPoolSize pool_sizes[] = {
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 },
        };
        VkDescriptorPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets = 1;
        pool_info.poolSizeCount = (ui32)(sizeof(pool_sizes) / sizeof(*(pool_sizes)));
        pool_info.pPoolSizes = pool_sizes;
        handleErr(vkCreateDescriptorPool(m_device, &pool_info, m_allocator.get(), &m_descriptor_pool));
    }
    void SdlVulkanRenderer::selectGraphicsQueueFamily() {
        ui32 queue_count;
        vkGetPhysicalDeviceQueueFamilyProperties(m_curr_gpu, &queue_count, nullptr);
        VkQueueFamilyProperties * queues = (VkQueueFamilyProperties*)malloc(sizeof(VkQueueFamilyProperties) * queue_count);
        vkGetPhysicalDeviceQueueFamilyProperties(m_curr_gpu, &queue_count, queues);
        for (ui32 cc = 0; cc < queue_count; cc++) {
            if (queues[cc].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                m_queue_family = cc;
                break;
            }
        }
        free(queues);
        assert(m_queue_family != ((ui32) - 1));
    }
    void SdlVulkanRenderer::selectPhysicalDevice() {
        ui32 gpu_count = 0;
        handleErr(vkEnumeratePhysicalDevices(m_instance, &gpu_count, nullptr));
        if (gpu_count == 0) {
            Logger::err_log() << "Critical Error: No Compatible Display Devices detected." << endl;
            std::exit(1);
        }
        m_gpus.resize(gpu_count);
        handleErr(vkEnumeratePhysicalDevices(m_instance, &gpu_count, m_gpus.data()));
        for (VkPhysicalDevice &dev : m_gpus) {
            VkPhysicalDeviceProperties dev_props;
            vkGetPhysicalDeviceProperties(dev, &dev_props);
            if (dev_props.deviceName == m_saved_dev_name) {
                m_curr_gpu = dev;
                return;
            }
        }
        m_curr_gpu = m_gpus[0];
        for (VkPhysicalDevice& dev : m_gpus) {
            VkPhysicalDeviceProperties dev_props;
            vkGetPhysicalDeviceProperties(dev, &dev_props);
            if (dev_props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
                m_curr_gpu = dev;
                break;
            }
        }
    }
    void SdlVulkanRenderer::cleanup() {
        vkDestroyDescriptorPool(m_device, m_descriptor_pool, m_allocator.get());
        vkDestroyDevice(m_device, m_allocator.get());
        vkDestroyInstance(m_instance, m_allocator.get());
    }
}