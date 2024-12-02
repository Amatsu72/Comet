#pragma once
#include "../events/Event.h"
#include "Context.h"

namespace Comet
{

    struct WindowProps
    {
        std::string title;
        unsigned int width;
        unsigned int height;
        bool vsync;

        WindowProps(const std::string &title = "Comet", unsigned int width = 1280, unsigned int height = 720, bool vsync = true)
            : title(title), width(width), height(height), vsync(vsync) {}
    };

    class Window
    {
    public:
        using EventCallbackFn = std::function<void(Event &)>;
        Window(const WindowProps &props);
        ~Window();

        void update();
        void shutdown();
        unsigned int getWidth() const { return m_props.width; }
        unsigned int getHeight() const { return m_props.height; }

        void setEventCallback(const EventCallbackFn &callback);
        void setVsync(bool enable) { m_props.vsync = enable; }
        bool isVsync() const { return m_props.vsync; }

        Window &get_native_window() { return *this; }

    private:
        WindowProps m_props;
        GLFWwindow *m_window;
        std::unique_ptr<Context> m_context;
    };
}