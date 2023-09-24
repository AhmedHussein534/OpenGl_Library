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
#include "engine/Cameras/PerspectiveCamera.hpp"
#include "engine/Elements/Texture.hpp"
#include "engine/Elements/Texture3D.hpp"
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
        // std::shared_ptr<PerspectiveCamera> m_camera = std::make_shared<PerspectiveCamera>(90.0f, 1920.0f/1080.0f, 0.1f, 100.0f);
        std::shared_ptr<OrthographicCamera> m_camera = std::make_shared<OrthographicCamera>(4.0f, -4.0f, -4.0f, 4.0f);
        std::vector<std::shared_ptr<Layout>> layouts = {};
        std::shared_ptr<Layout> layout = std::make_shared<Layout>();

        auto texture3D = std::make_shared<StbTexture3D>("res/smile.png")->getTex();
        auto model = texture3D->getModel();

        layout->addElement(texture3D);
        layouts.push_back(layout);
        bool shouldBreak = false;
        while (!shouldClose)
        {
             *model = glm::rotate(*model, glm::radians(1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            /*
            static float m_x = -1.0f;
            static float m_y = 0.5f;
            static float m_z = 1.0f;

            float time = glfwGetTime();
            m_x = sin(time) / 2.0f;
            m_z = cos(time) / 2.0f;

            m_camera->setDirection({m_x, m_y, m_z}, { 0.0f, 0.0f, 0.0f });
            */
            for (auto& l : layouts)
            {
                if (!shouldClose)
                {
                     l->draw(m_camera);
                    /* Swap front and back buffers */
                    window->OnUpdate();
                }
            }
        }
    }

    return 0;
}