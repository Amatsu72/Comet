#pragma once
#include "../graphics/VkCommand.h"
#include <GLFW/glfw3.h>

namespace Comet
{

    class Context
    {
    public:
        Context(GLFWwindow *window) : m_windowHandle(window) {}
        ~Context();

        void init();
        void swapBuffers();

    private:
        void createInstance();
        void pickupPhysicalDevice();
        void createDevice();
        void createSurface();

        vk::Instance m_instance;
        vk::PhysicalDevice m_physicalDevice;
        vk::Device m_device;
        vk::SurfaceKHR m_surface;

        GLFWwindow *m_windowHandle;
    };
}