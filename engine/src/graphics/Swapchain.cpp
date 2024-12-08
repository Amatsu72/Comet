#include "Swapchain.h"
#include "../core/Context.h"

namespace Comet
{
    Swapchain::Swapchain(Context *context) : m_contextHandle(context)
    {
        init();
    }
    Swapchain::~Swapchain()
    {
    }

    void Swapchain::init()
    {
        querySwapchainInfo();
    }
    void Swapchain::querySwapchainInfo()
    {
        }
}
