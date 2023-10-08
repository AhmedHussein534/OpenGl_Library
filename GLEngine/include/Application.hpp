#pragma once

#include <memory>
#include <functional>
#include <iostream>
#include <assert.h>

#include "events/EventDispatcher.hpp"
#include "Timestep.hpp"
#include "Elements/Rectangle.hpp"
#include "Elements/Texture.hpp"
#include "Renderer2D.hpp"


namespace GL_ENGINE
{
    class WindowsWindow;

    class Application
    {
        public:
        Application(const std::string &appName, uint32_t length, uint32_t width);

        virtual void onDeltaStep() = 0;

        virtual bool onKeyPressed(const KeyPressedEvent &e) = 0;

        virtual bool onCloseTriggered(const WindowCloseEvent &e) = 0;

        bool onWindowResize(const WindowResizeEvent &e);

        bool onFocusChange(const WindowFocusEvent &e);

        void mainloop();

        void onEvent(Event& e);

        void updateFps(float fps);

        void resetFps();

        float getFps();

        private:
        void init();

        void recalculateDelta();


        protected:
        bool m_gameRunning = true;
        bool loopRunning = true;
        bool isMinimized = false;
        bool isFocused = true;
        std::string m_appName;
        uint32_t m_length;
        uint32_t m_width;


        private:
        const float sleepToDelta = 0.05f;
        float m_fps = 6;
        float deltaTime_ms = 1000.0f / m_fps;
        float sleepTime_ms = sleepToDelta * deltaTime_ms;
        std::shared_ptr<WindowsWindow> window;
    };
}
