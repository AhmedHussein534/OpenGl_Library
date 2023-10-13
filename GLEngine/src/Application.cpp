
#include <windows.h>

#include "Application.hpp"
#include <external/glew/include/GL/glew.h>
#include "window/Windows/WindowsWindow.hpp"

namespace GL_ENGINE
{
    Application::Application(const std::string &appName, uint32_t length, uint32_t width) : m_appName(appName),
                                                                                            m_length(length),
                                                                                            m_width(width)

    {
        init();
        EventDispatcher::getInstance().subscribeToEvents(EventCategoryKeyboard | EventCategoryApplication,
                                                         std::bind(&Application::onEvent, this, std::placeholders::_1));
    }

    void Application::mainloop()
    {
        GL_ENGINE::Timestep time;
        while(loopRunning)
        {
            auto delta = time.getDelta<std::milli>();
            float isDeltaTime = delta >= deltaTime_ms - sleepTime_ms;
            if (isDeltaTime)
            {
                time.notifyUpdate();
                if (m_gameRunning && isFocused && !isMinimized)
                {
                    onDeltaStep();
                }

                window->OnUpdate();
            }

            Sleep(static_cast<DWORD>(sleepTime_ms));
        }
    }

    void Application::onEvent(Event& e)
    {
        EventDispatcher::getInstance().Dispatch<KeyPressedEvent>(e, std::bind(&Application::onKeyPressed, this, std::placeholders::_1));
        EventDispatcher::getInstance().Dispatch<WindowCloseEvent>(e, std::bind(&Application::onCloseTriggered, this, std::placeholders::_1));
        EventDispatcher::getInstance().Dispatch<WindowResizeEvent>(e, std::bind(&Application::onWindowResize, this, std::placeholders::_1));
        EventDispatcher::getInstance().Dispatch<WindowFocusEvent>(e, std::bind(&Application::onFocusChange, this, std::placeholders::_1));
    }



    void Application::init()
    {
        window = std::make_shared<WindowsWindow>(WindowProps{m_appName, m_length, m_width});
        if (!window)
        {
            std::cout << "ERROR: failed to create window" << std::endl;
            assert(0);
        }

        GLenum err = glewInit();
        if (err != GLEW_OK)
        {
            std::cout << "ERROR: "
                << glewGetErrorString(err)
                << std::endl;
            assert(0);
        }
    }

    void Application::recalculateDelta()
    {
        deltaTime_ms = 1000.0f / m_fps;
        sleepTime_ms = sleepToDelta * deltaTime_ms;
    }


    void Application::updateFps(float fps)
    {
        m_fps = fps;
        recalculateDelta();
    }

    float Application::getFps()
    {
        return m_fps;
    }

    void Application::resetFps()
    {
        m_fps = 60;
        recalculateDelta();
    }

    bool Application::onWindowResize(const WindowResizeEvent &e)
    {
        m_width = e.GetHeight();
        m_length = e.GetWidth();
        isMinimized = ((m_width == 0) && (m_length == 0));
        return true;
    }

    bool Application::onFocusChange(const WindowFocusEvent &e)
    {
        std::cout << e.ToString() << std::endl;
        isFocused = e.isFocused();
        return true;
    }
};