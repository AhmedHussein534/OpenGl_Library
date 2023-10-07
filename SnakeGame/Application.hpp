#pragma once

#include <memory>
#include <functional>
#include <iostream>
#include <assert.h>

#include <GL/glew.h>
#include "events/EventDispatcher.hpp"
#include "window/Windows/WindowsWindow.hpp"

#include "engine/Timestep.hpp"
#include "engine/Elements/Rectangle.hpp"
#include "engine/Elements/Texture.hpp"
#include "engine/Renderer2D.hpp"


namespace GL_ENGINE
{
    class Application
    {
        public:
        Application(const std::string &appName, uint32_t length, uint32_t width);

        virtual void onDeltaStep() = 0;

        virtual bool onKeyReleased(const KeyReleasedEvent &e) = 0;

        virtual bool onCloseTriggered(const WindowCloseEvent &e) = 0;

        void mainloop();

        void onEvent(Event& e);

        void updateFps(float fps);

        float getFps();

        private:
        void init(const std::string &appName, uint32_t length, uint32_t width);


        protected:
        bool m_gameRunning = true;
        bool loopRunning = true;

        private:
        const float sleepToDelta = 0.05f;
        float m_fps = 6;
        float deltaTime_ms = 1000.0f / m_fps;
        float sleepTime_ms = sleepToDelta * deltaTime_ms;
        std::shared_ptr<WindowsWindow> window;
    };
}
