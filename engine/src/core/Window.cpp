#include "Window.h"
#include "Log.h"

namespace Comet
{

    Window::Window(const WindowProps &props) : m_props(props)
    {
        if (!glfwInit())
        {
            LOG_ERROR("GLFW initialization failed!");
            return;
        }
        
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        m_window = glfwCreateWindow(m_props.width, m_props.height, m_props.title.c_str(), nullptr, nullptr);

        if(!m_window)
        {
            LOG_ERROR("failed to createGLFW window!");
            return;
        }

        m_context = std::make_unique<Context>(m_window);
        if(!m_context) 
        {
            LOG_ERROR("failed to create GLFW context!");
            return;
        }
        m_context->init();
    }

    Window::~Window()
    {
        shutdown();
    }

    void Window::update()
    {
        while (!glfwWindowShouldClose(m_window))
        {
            glfwPollEvents();
            glfwSwapBuffers(m_window);
        }
    }
    void Window::shutdown()
    {
        glfwDestroyWindow(m_window);
        glfwTerminate();
        LOG_INFO("GLFW window destroyed!");
    }
}