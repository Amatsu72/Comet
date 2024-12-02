#include "Context.h"

namespace Comet
{
    Context::~Context()
    {
        m_instance.destroy();
        m_device.destroy();
    }
    void Context::init()
    {
        glfwMakeContextCurrent(m_windowHandle);
        createInstance();
        pickupPhysicalDevice();
        createDevice();
    }

    

    void Context::swapBuffers()
    {
        glfwSwapBuffers(m_windowHandle);
    }

    void Context::createInstance()
    {
        // 1. 构建layers
        auto availableLayers = vk::enumerateInstanceLayerProperties();
        // CHECK_VK(vkEnumerateInstanceLayerProperties(&avaliableLayerCount, availableLayers.data()));
    }

    void Context::pickupPhysicalDevice() {

    }
    void Context::createDevice() {

    }
}