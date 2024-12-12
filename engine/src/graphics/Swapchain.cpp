#include "Swapchain.h"
#include "../core/Context.h"

namespace Comet
{
    Swapchain::Swapchain(Context *context, uint32_t width, uint32_t height) : m_contextHandle(context)
    {
        m_surface = context->getSurface();
        init(width, height);
        createImages();
    }
    Swapchain::~Swapchain()
    {
        for (auto &image : m_images)
        {
            m_contextHandle->getDevice().destroyImageView(image.view);
        }
        m_contextHandle->getDevice().destroySwapchainKHR(m_swapchain);
        m_contextHandle->getInstance().destroySurfaceKHR(m_surface);
    }

    void Swapchain::init(uint32_t width, uint32_t height)
    {
        querySwapchainInfo(width, height);
        LOG_TRACE("{0}, : imageExtent : {1} x {2}", __FUNCTION__, m_surfaceInfo.imageExtent.width, m_surfaceInfo.imageExtent.height);
        // LOG_TRACE("{0}, : surfaceFormat : {1}", __FUNCTION__, m_surfaceInfo.format);
        // LOG_TRACE("{0}, : presentMode : {1}", __FUNCTION__, m_surfaceInfo.presentMode);
        vk::SwapchainCreateInfoKHR swapchainCreateInfo;
        swapchainCreateInfo.setClipped(true)
            .setImageArrayLayers(1)
            .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
            .setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)
            .setSurface(m_surface)
            .setImageColorSpace(m_surfaceInfo.format.colorSpace)
            .setImageFormat(m_surfaceInfo.format.format)
            .setImageExtent(m_surfaceInfo.imageExtent)
            .setMinImageCount(m_surfaceInfo.imageCount)
            .setPresentMode(m_surfaceInfo.presentMode);

        auto queueFamilyIndices = m_contextHandle->getQueueFamilyIndices();
        if (queueFamilyIndices.graphicsQueue.value() == queueFamilyIndices.presentQueue.value())
        {
            swapchainCreateInfo.setQueueFamilyIndices(queueFamilyIndices.graphicsQueue.value())
                .setImageSharingMode(vk::SharingMode::eExclusive);
        }
        else
        {
            std::array<uint32_t, 2> queueFamilyIndicesArray = {queueFamilyIndices.graphicsQueue.value(), queueFamilyIndices.presentQueue.value()};
            swapchainCreateInfo.setQueueFamilyIndices(queueFamilyIndicesArray)
                .setImageSharingMode(vk::SharingMode::eConcurrent);
        }

        m_swapchain = m_contextHandle->getDevice().createSwapchainKHR(swapchainCreateInfo);
        LOG_INFO("{0}, : Swapchain: {1}", __FUNCTION__, (void *)m_swapchain);
    }

    void Swapchain::querySwapchainInfo(uint32_t width, uint32_t height)
    {
        const auto &physicalDevice = m_contextHandle->getPhysicalDevice();
        m_surfaceInfo.capabilities = physicalDevice.getSurfaceCapabilitiesKHR(m_surface);

        auto formats = physicalDevice.getSurfaceFormatsKHR(m_surface);
        m_surfaceInfo.format = formats[0];
        for (const auto &format : formats)
        {
            if (format.format == vk::Format::eB8G8R8A8Srgb && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
            {
                m_surfaceInfo.format = format;
                break;
            }
        }

        m_surfaceInfo.imageCount = std::clamp<uint32_t>(2, m_surfaceInfo.capabilities.minImageCount, m_surfaceInfo.capabilities.maxImageCount);
        m_surfaceInfo.imageExtent.width = std::clamp<uint32_t>(width, m_surfaceInfo.capabilities.minImageExtent.width, m_surfaceInfo.capabilities.maxImageExtent.width);
        m_surfaceInfo.imageExtent.height = std::clamp<uint32_t>(height, m_surfaceInfo.capabilities.minImageExtent.height, m_surfaceInfo.capabilities.maxImageExtent.height);

        const auto &presentModes = physicalDevice.getSurfacePresentModesKHR(m_surface);
        m_surfaceInfo.presentMode = vk::PresentModeKHR::eFifo;
        for (const auto &presentMode : presentModes)
        {
            if (presentMode == vk::PresentModeKHR::eMailbox)
            {
                m_surfaceInfo.presentMode = presentMode;
                break;
            }
        }
    }
    void Swapchain::createImages()
    {
        auto images = m_contextHandle->getDevice().getSwapchainImagesKHR(m_swapchain);
        for (auto &image : images)
        {
            Image img;
            img.image = image;
            vk::ImageViewCreateInfo viewCreateInfo;
            vk::ComponentMapping mapping;
            vk::ImageSubresourceRange range;
            range.setBaseArrayLayer(0)
                .setBaseMipLevel(0)
                .setLayerCount(1)
                .setLevelCount(1)
                .setAspectMask(vk::ImageAspectFlagBits::eColor);
            viewCreateInfo.setImage(image)
                .setFormat(m_surfaceInfo.format.format)
                .setViewType(vk::ImageViewType::e2D)
                .setSubresourceRange(range)
                .setComponents(mapping);
            img.view = m_contextHandle->getDevice().createImageView(viewCreateInfo);
            m_images.push_back(img);
        }
        LOG_TRACE("{0}, : images size: {1}", __FUNCTION__, m_images.size());
    }

}
