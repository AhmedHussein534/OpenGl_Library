#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <memory>
#include <chrono>
#include <windows.h>
#include <unistd.h>
#include <bits/stdc++.h>

#include "external\glm\glm\gtx\rotate_vector.hpp"
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
#include "engine/Timestep.hpp"
#include "events/EventDispatcher.hpp"

#include "window/Windows/WindowsWindow.hpp"

#define LOG_DEBUG std::cout

#define SWAP_INTERVAL 1


using WormPiece = Square;
using WormPiecePtr = std::shared_ptr<WormPiece>;
using Worm = std::vector<std::shared_ptr<WormPiece>>;

enum class MOVE_DIRECTION : uint32_t
{
    UP,
    DOWN,
    RIGHT,
    LEFT,
};

bool shouldClose = false;
const uint32_t wormLen = 4;
const float wormPieceSize = 0.045f;
const float wormPieceVisible = 0.045f * 0.8f;
const float startYPos = wormPieceSize / 2.0f;
MOVE_DIRECTION moveDirection = MOVE_DIRECTION::UP;
WormPiece* food = nullptr;

glm::vec3 getDirectionUnitVector(MOVE_DIRECTION direction)
{
    switch(direction)
    {
        case MOVE_DIRECTION::UP: {
            return glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));
        }
        case MOVE_DIRECTION::DOWN: {
            return glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f));
        }
        case MOVE_DIRECTION::RIGHT: {
            return glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f));
        }
        case MOVE_DIRECTION::LEFT: {
            return glm::normalize(glm::vec3(-1.0f, 0.0f, 0.0f));
        }

        default:
            return glm::vec3(0.0f);
    }
}
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


bool onCloseTriggered(const WindowCloseEvent &e)
{
    shouldClose = true;
    return true;
}

bool onKeyReleased(const KeyReleasedEvent &e)
{

    switch (e.GetKeyCode())
    {
        case Key::Up:
            moveDirection = MOVE_DIRECTION::UP;
            break;
        case Key::Down:
            moveDirection = MOVE_DIRECTION::DOWN;
            break;
        case Key::Right:
            moveDirection = MOVE_DIRECTION::RIGHT;
            break;
        case Key::Left:
            moveDirection = MOVE_DIRECTION::LEFT;
            break;
        default:
            return true;
    }

    return true;
}


void onEvent(Event& e)
{
    std::cout << e.GetName() << std::endl;
    EventDispatcher::getInstance().Dispatch<KeyReleasedEvent>(e, onKeyReleased);
    EventDispatcher::getInstance().Dispatch<WindowCloseEvent>(e, onCloseTriggered);

}


void initWorm(Worm &worm,  std::shared_ptr<Layout> layout)
{
    float currentXPiecePos = 0.0f - (wormPieceSize / 2.0f);
    for (uint32_t i = 0; i < wormLen; i++)
    {
        auto piece = std::make_shared<Square>(currentXPiecePos, startYPos, wormPieceVisible); //only draw 80% of each size to leave gap
        worm.push_back(piece);
        layout->addElement(piece);
        currentXPiecePos += wormPieceSize;
    }

}

void moveWorm(Worm &worm, float step = wormPieceSize, MOVE_DIRECTION direction = MOVE_DIRECTION::UP)
{
    for (auto it = worm.begin(); it != worm.end(); it++ )
    {
        auto pieceModel = (*it)->getModel();
        auto center = (*it)->getCenter();
        if ((it + 1) != worm.end())
        {
            glm::vec3 nextPieceCenter = (*(it + 1))->getCenter();
            *pieceModel = glm::translate(*pieceModel, glm::normalize(nextPieceCenter - center) * step);
        }
        else
        {
            // HEAD
            auto dirUnitVector = getDirectionUnitVector(direction);
            *pieceModel = glm::translate(*pieceModel, dirUnitVector * step);
        }
    }

}

void createRandomFood(std::shared_ptr<Layout> layout)
{
    std::default_random_engine gen;
    std::uniform_real_distribution<float> distribution(-0.9,
                                                   0.9);
    auto foodShared= std::make_shared<WormPiece>(distribution(gen), distribution(gen), wormPieceVisible);

    layout->addElement(foodShared);
    food = foodShared.get();
}

int main(void)
{

    EventDispatcher::getInstance().subscribeToEvents(EventCategoryKeyboard | EventCategoryApplication, onEvent);
    auto window = init();
    float aspectRatio = static_cast<float>(window->GetWidth()) / static_cast<float>(window->GetHeight());
    Timestep time;
    if (window == nullptr)
    {
        return -1;
    }


    std::shared_ptr<OrthographicCamera> m_camera = std::make_shared<OrthographicCamera>(-1.0f * aspectRatio, 1.0f * aspectRatio, -1.0f, 1.0f);
    std::vector<std::shared_ptr<Layout>> layouts = {};
    std::shared_ptr<Layout> wormLayout = std::make_shared<Layout>();
    Worm worm = {};
    initWorm(worm, wormLayout);
    createRandomFood(wormLayout);
    layouts.push_back(wormLayout);

    while (!shouldClose)
    {
        for (auto& l : layouts)
        {
            if (!shouldClose)
            {
                l->draw(m_camera);
                /* Swap front and back buffers */
                window->OnUpdate();
            }
        }

        moveWorm(worm, wormPieceSize, moveDirection);
        usleep(100000);
    }




    return 0;
}