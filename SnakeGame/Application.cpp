
#include <windows.h>

#include "Application.hpp"



namespace GL_ENGINE
{
    Application::Application(const std::string &appName, int length, int width)
    {
        init(appName, length, width);
        EventDispatcher::getInstance().subscribeToEvents(EventCategoryKeyboard | EventCategoryApplication,
                                                         std::bind(onEvent, this, std::placeholders::_1));
    }

    void Application::mainloop()
    {
        GL_ENGINE::Timestep time;
        while(loopRunning)
        {
            auto delta = time.getDelta<std::milli>();
            if (delta >= deltaTime_ms - sleepTime_ms)
            {
                time.notifyUpdate();
                if (m_gameRunning)
                {
                    onDeltaStep();
                }

                window->OnUpdate();
            }

            Sleep(sleepTime_ms);
        }
    }

    void Application::onEvent(Event& e)
    {
        EventDispatcher::getInstance().Dispatch<KeyReleasedEvent>(e, std::bind(onKeyReleased, this, std::placeholders::_1));
        EventDispatcher::getInstance().Dispatch<WindowCloseEvent>(e, std::bind(onCloseTriggered, this, std::placeholders::_1));
    }



    void Application::init(const std::string &appName, int length, int width)
    {
        window = std::make_shared<WindowsWindow>(WindowProps{appName, length, width});
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

    void Application::updateFps(float fps)
    {
        m_fps = fps;
        deltaTime_ms = 1000.0f / m_fps;
        sleepTime_ms = sleepToDelta * deltaTime_ms;
    }

    float Application::getFps()
    {
        return m_fps;
    }
};