#include "../engine/src/pch.h"
#include "Application.h"

int main()
{
    App::app.init();
    // std::cout << "Comet Engine initialized" << std::endl;
    Comet::Log::init();
    LOG_INFO("Comet Engine initialized");
    LOG_DEBUG("Comet Engine initialized");
    LOG_TRACE("Comet Engine initialized");
    LOG_WARN("Comet Engine initialized");
    LOG_ERROR("Comet Engine initialized");
    while (App::app.isRunning())
    {
        App::app.update();
    }

    return 0;
}