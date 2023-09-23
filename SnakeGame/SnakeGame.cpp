#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <memory>

#include "engine/IndexBuffer.hpp"
#include "engine/VertexBuffer.hpp"
#include "engine/VertexArray.hpp"
#include "engine/Shader.hpp"
#include "engine/Layout.hpp"
#include "engine/Elements/Square.hpp"
#include "engine/Elements/Cube.hpp"
#include "engine/Cameras/OrthographicCamera.hpp"
#include "StbTexture.hpp"

#include "events/EventDispatcher.hpp"
#include "window/Windows/WindowsWindow.hpp"
#define LOG_DEBUG std::cout

#define SWAP_INTERVAL 1

std::shared_ptr<WindowsWindow> init()
{
    std::shared_ptr<WindowsWindow> window = std::make_shared<WindowsWindow>(WindowProps{"Mywindow", 1920, 1080});
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        std::cout << "ERROR: "
            << glewGetErrorString(err)
            << std::endl;
    }

    return window;
}



bool shouldClose = false;

bool onCloseTriggered(const WindowCloseEvent &e)
{
    shouldClose = true;
    return true;
}

void onEvent(Event& e)
{
    std::cout << e.ToString() << std::endl;
    EventDispatcher::getInstance().Dispatch<WindowCloseEvent>(e, onCloseTriggered);
}


int main(void)
{
    EventDispatcher::getInstance().subscribeToEvents(EventCategoryKeyboard | EventCategoryApplication, onEvent);
    auto window = init();
    if (window == nullptr)
    {
        return -1;
    }

    {
        std::shared_ptr<OrthographicCamera> m_camera = std::make_shared<OrthographicCamera>(-1.0f, 1.0f, -1.0f, 1.0f);
        std::vector<std::shared_ptr<Layout>> layouts = {};
        std::shared_ptr<Layout> layout = std::make_shared<Layout>(m_camera);
        layout->addElement(std::make_shared<Cube>(-0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f));
        layouts.push_back(layout);
        bool shouldBreak = false;
        while (!shouldClose)
        {
            for (auto& l : layouts)
            {
                if (!shouldClose)
                {
                     l->draw();
                    /* Swap front and back buffers */
                    window->OnUpdate();
                }
            }
        }
    }

    return 0;
}