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
#include "StbTexture.hpp"

#define LOG_DEBUG std::cout

#define SWAP_INTERVAL 1

GLFWwindow* createWindow()
{
    GLFWwindow* window;

    /* USE CORE INSTEAD OF CAP */
    /* NOTE: CORE NEEDS VERTEX ARRAY TO BE CREATED */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1920, 1080, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return nullptr;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Update window every 1 second*/
    glfwSwapInterval(SWAP_INTERVAL);
    return window;
}

GLFWwindow* init()
{
    GLFWwindow* ret = nullptr;
    /* Initialize the library */
    do
    {
        if (!glfwInit())
        {
            break;
        }

        GLFWwindow* window = createWindow();

        /* Init Glew wrapper for OPENGL*/
        GLenum err = glewInit();
        if (err != GLEW_OK)
        {
            std::cout << "ERROR: "
                << glewGetErrorString(err)
                << std::endl;
            break;
        }

        ret = window;
        std::cout << glGetString(GL_VERSION) << std::endl;
    } while (0);


    return ret;
}

int main(void)
{
    GLFWwindow* window = init();
    if (window == nullptr)
    {
        return -1;
    }

    {

        std::vector<std::shared_ptr<Layout>> layouts = {};
        std::shared_ptr<Layout> layout = std::make_shared<Layout>();
        layout->addElement(std::make_shared<Cube>(-1.0f, 1.0f, 0.25f, 0.0f, 0.0f, 1.0f, 1.0f));
        layouts.push_back(layout);
        bool shouldBreak = false;
        while (true)
        {
            for (auto& l : layouts)
            {
                l->draw();
                /* Swap front and back buffers */
                glfwSwapBuffers(window);

                /* Poll for and process events */
                glfwPollEvents();

                if (glfwWindowShouldClose(window))
                {
                    shouldBreak = true;
                    break;

                }
            }

            if (shouldBreak)
            {
                break;
            }
        }
    }

    glfwTerminate();
    return 0;
}