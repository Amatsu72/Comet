#include "Context.h"
#include <vulkan/vulkan_macos.h>

namespace Comet
{
    static VkBool32 vkDebugCallback(VkDebugReportFlagsEXT flags,
                                    VkDebugReportObjectTypeEXT objectType,
                                    uint64_t object,
                                    size_t location,
                                    int32_t messageCode,
                                    const char *pLayerPrefix,
                                    const char *pMessage,
                                    void *pUserData)
    {
        if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
        {
            LOG_ERROR("{0}", pMessage);
        }
        else if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT || flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)
        {
            LOG_WARN("{0}", pMessage);
        }
        return VK_TRUE;
    }

    Context::Context(GLFWwindow *window) : m_windowHandle(window)
    {
    }

    Context::~Context()
    {
        m_instance.destroySurfaceKHR(m_surface);
        m_device.destroy();
        m_instance.destroy();
    }

    void Context::init()
    {
        glfwMakeContextCurrent(m_windowHandle);
        createInstance();
        createSurface();
        selectPhysicalDevice();
        queryQueueFamilies();
        createDevice();
        getQueues();
        m_swapchain = std::make_unique<Swapchain>(this, 720, 1080);
    }

    void Context::swapBuffers()
    {
        glfwSwapBuffers(m_windowHandle);
    }

    void Context::createInstance()
    {
        // 构建layers
        auto availableLayers = vk::enumerateInstanceLayerProperties();
        LOG_INFO("Available Layers:");
        for (auto &layer : availableLayers)
        {
            std::cout << "Comet Engine: " << layer.layerName << std::endl;
        }
#ifdef DEBUG
        std::vector<const char *> layers = {"VK_LAYER_KHRONOS_validation"};
#else
        std::vector<const char *> layers = {};
#endif
        // 构建extensions
        auto availableExtensions = vk::enumerateInstanceExtensionProperties();
        LOG_INFO("Available Extensions:");
        for (auto &extension : availableExtensions)
        {
            std::cout << "Comet Engine: " << extension.extensionName << std::endl;
        }
        uint32_t glfwExtensionCount = 0;
        const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        for (int i = 0; i < glfwExtensionCount; i++)
        {
            const char *extension = glfwExtensions[i];
            LOG_INFO("glfw Extensions: {0}", extension);
        }
        std::vector<const char *> enabledExtensions = {
            VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME,
            VK_KHR_SURFACE_EXTENSION_NAME,       // 必要的表面扩展
            VK_MVK_MACOS_SURFACE_EXTENSION_NAME, // macOS 平台特有扩展
            "VK_EXT_debug_report"};
        enabledExtensions.insert(enabledExtensions.end(), glfwExtensions, glfwExtensions + glfwExtensionCount);

        // 构建instance
        vk::InstanceCreateInfo createInfo;

        vk::ApplicationInfo appInfo;
        appInfo.setApiVersion(VK_API_VERSION_1_3)
            .setPApplicationName("Comet");

        VkDebugReportCallbackCreateInfoEXT debugCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT,
            .flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT,
            .pfnCallback = vkDebugCallback};
        createInfo.setPApplicationInfo(&appInfo)
            .setPEnabledLayerNames(layers)
            .setEnabledExtensionCount(static_cast<uint32_t>(enabledExtensions.size()))
            .setPEnabledExtensionNames(enabledExtensions)
            .setFlags(vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR)
            .setPNext(&debugCreateInfo);
        m_instance = vk::createInstance(createInfo);
        LOG_TRACE("{0}, : instance : {1}", __FUNCTION__, (void *)m_instance);
    }

    void Context::selectPhysicalDevice()
    {
        auto devices = m_instance.enumeratePhysicalDevices();
        m_physicalDevice = devices[0];
        std::cout << "Comet Engine: " << "physical device : " << m_physicalDevice.getProperties().deviceName << std::endl;
        m_physicalDeviceMemoryProperties = m_physicalDevice.getMemoryProperties();
    }

    void Context::createDevice()
    {
        vk::DeviceCreateInfo createInfo;
        std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
        float graphicsQueuePriority = 0.0f;
        float presentQueuePriority = 1.0f;
        uint32_t queueCreateInfoCount = 1;
        if (m_queueFamilyIndices.presentQueue.value() == m_queueFamilyIndices.graphicsQueue.value())
        {
            vk::DeviceQueueCreateInfo queueCreateInfo;
            queueCreateInfo.setQueuePriorities(graphicsQueuePriority)
                .setQueueCount(1)
                .setQueueFamilyIndex(m_queueFamilyIndices.graphicsQueue.value());
            queueCreateInfos.push_back(std::move(queueCreateInfo));
        }
        else
        {
            vk::DeviceQueueCreateInfo queueCreateInfo;
            queueCreateInfo.setQueuePriorities(graphicsQueuePriority)
                .setQueueCount(1)
                .setQueueFamilyIndex(m_queueFamilyIndices.graphicsQueue.value());
            queueCreateInfos.push_back(queueCreateInfo);
            queueCreateInfo.setQueuePriorities(presentQueuePriority)
                .setQueueCount(1)
                .setQueueFamilyIndex(m_queueFamilyIndices.presentQueue.value());
            queueCreateInfos.push_back(queueCreateInfo);
            queueCreateInfoCount = 2;
        }
        // device extensions
        std::vector<const char *> extensions = {
            "VK_KHR_portability_subset",
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        };
        createInfo.setQueueCreateInfoCount(queueCreateInfoCount)
            .setQueueCreateInfos(queueCreateInfos)
            .setPEnabledExtensionNames(extensions);
        m_device = m_physicalDevice.createDevice(createInfo);
        LOG_TRACE("{0}, : device : {1}", __FUNCTION__, (void *)m_device);
    }

    void Context::createSurface()
    {
        glfwCreateWindowSurface(m_instance, m_windowHandle, nullptr, (VkSurfaceKHR *)&m_surface);
        LOG_TRACE("{0}, : surface : {1}", __FUNCTION__, (void *)m_surface);
    }
    void Context::queryQueueFamilies()
    {
        auto propertis = m_physicalDevice.getQueueFamilyProperties();
        for (int i = 0; i < propertis.size(); i++)
        {
            const auto &property = propertis[i];
            if (property.queueFlags & vk::QueueFlagBits::eGraphics)
            {
                m_queueFamilyIndices.graphicsQueue = i;
            }
            if (m_physicalDevice.getSurfaceSupportKHR(i, m_surface))
            {
                m_queueFamilyIndices.presentQueue = i;
            }
            if (m_queueFamilyIndices)
            {
                break;
            }
        }
        LOG_TRACE("{0}, : graphicsQueue : {1}, presentQueue : {2}", __FUNCTION__, m_queueFamilyIndices.graphicsQueue.value(), m_queueFamilyIndices.presentQueue.value());
    }
    void Context::getQueues()
    {
        m_graphicsQueue = m_device.getQueue(m_queueFamilyIndices.graphicsQueue.value(), 0);
        m_presentQueue = m_device.getQueue(m_queueFamilyIndices.presentQueue.value(), 0);
        LOG_TRACE("{0}, : graphicsQueue : {1}, presentQueue : {2}", __FUNCTION__, (void *)m_graphicsQueue, (void *)m_presentQueue);
    }
}