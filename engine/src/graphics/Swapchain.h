#pragma once
#include <vulkan/vulkan.hpp>

namespace Comet
{
    class Context;

    class Swapchain
    {
    public:
        Swapchain(Context *context);
        ~Swapchain();

        struct SwapchainInfo
        {
            vk::Extent2D imageExtent;
            uint32_t imageCount;
            vk::Format format;
        };

    private:
        void init();
        void querySwapchainInfo();

        Context *m_contextHandle;
        SwapchainInfo m_swapchainInfo;
        vk::SwapchainKHR m_swapchain;
    };
}