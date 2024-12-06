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
            LOG_WARN("{1}", pMessage);
        }
        return VK_TRUE;
    }

    Context::~Context()
    {
        m_device.destroy();
        m_instance.destroy();
    }

    void Context::init()
    {
        glfwMakeContextCurrent(m_windowHandle);
        createInstance();
        createSurface();
        pickupPhysicalDevice();
        createDevice();
    }

    void Context::swapBuffers()
    {
        glfwSwapBuffers(m_windowHandle);
    }

    void Context::createInstance()
    {
        // 构建layers
        auto availableLayers = vk::enumerateInstanceLayerProperties();
        for (auto &layer : availableLayers)
        {
            std::cout << layer.layerName << std::endl;
        }
#ifdef DEBUG
        std::vector<const char *> layers = {"VK_LAYER_KHRONOS_validation"};
#else
        std::vector<const char *> layers = {};
#endif
        // 构建extensions
        auto availableExtensions = vk::enumerateInstanceExtensionProperties();
        for (auto &extension : availableExtensions)
        {
            std::cout << extension.extensionName << std::endl;
        }
        std::vector<const char *> enabledExtensions = {
            VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME,
            VK_KHR_SURFACE_EXTENSION_NAME,       // 必要的表面扩展
            VK_MVK_MACOS_SURFACE_EXTENSION_NAME, // macOS 平台特有扩展
            "VK_EXT_debug_report"};

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
    }

    void Context::pickupPhysicalDevice()
    {
        auto devices = m_instance.enumeratePhysicalDevices();
        m_physicalDevice = devices[0];
        std::cout << m_physicalDevice.getProperties().deviceName << std::endl;
    }

    void Context::createDevice()
    {
        vk::DeviceCreateInfo createInfo;
        vk::DeviceQueueCreateInfo queueCreateInfo;
        std::vector<const char *> extensions = {"VK_KHR_portability_subset"};
        createInfo.setQueueCreateInfoCount(1)
            .setPQueueCreateInfos(&queueCreateInfo)
            .setPEnabledExtensionNames(extensions);
        m_device = m_physicalDevice.createDevice(createInfo);
    }

    void Context::createSurface()
    {
        glfwCreateWindowSurface(m_instance, m_windowHandle, nullptr, (VkSurfaceKHR *)&m_surface);
        LOG_TRACE("{0}, : surface : {1}", __FUNCTION__, (void *)m_surface);
    }
}