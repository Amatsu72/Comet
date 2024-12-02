#include "Application.h"

namespace App
{

    Application::~Application()
    {
        m_window->shutdown();
    }

    void Application::update()
    {
        m_window->update();
    }

    void Application::init()
    {
        m_running = true;
        Comet::WindowProps props("Comet", 1280, 720, true);
        m_window = std::make_unique<Comet::Window>(props);
        // m_window->setEventCallback(BIND_EVENT_FN(Application::onEvent));
    }
}