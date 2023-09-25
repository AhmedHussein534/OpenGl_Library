#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <list>
#include <memory>
#include <chrono>
#include <windows.h>
#include <unistd.h>
#include <bits/stdc++.h>
#include <random>
#include <vector>

#include "engine/Layout.hpp"
#include "engine/Elements/Square.hpp"
#include "engine/Cameras/OrthographicCamera.hpp"
#include "engine/Timestep.hpp"
#include "events/EventDispatcher.hpp"


#include "window/Windows/WindowsWindow.hpp"


#define SWAP_INTERVAL 1


using WormPiece = Square;
using WormPiecePtr = std::shared_ptr<WormPiece>;
using Worm = std::list<std::shared_ptr<WormPiece>>;

enum class MOVE_DIRECTION : uint32_t
{
    UP,
    DOWN,
    RIGHT,
    LEFT,
};


bool shouldClose = false;
const float coordinateSize = 1000.0f;
const uint32_t wormLen = 4;
const float wormPieceSize = 0.04 *  coordinateSize;// always make sure this is divisible by 2
const float wormPieceVisible = wormPieceSize * 0.8f;
const float startYPos = wormPieceSize / 2.0f;
const float frameInterval_ms = 100;
MOVE_DIRECTION moveDirection = MOVE_DIRECTION::UP;


template <typename T>
bool isTwoPiecesCollided(std::shared_ptr<T> p1, std::shared_ptr<T> p2);


template<>
bool isTwoPiecesCollided(std::shared_ptr<Square> p1, std::shared_ptr<Square> p2)
{
    if ((p1 == nullptr) || (p2 == nullptr))
    {
        return false;
    }

    auto p1Center = p1->getCenter();
    auto p2Center = p2->getCenter();
    int p1_0 = static_cast<int>(p1Center[0]);
    int p2_0 = static_cast<int>(p2Center[0]);
    int p1_1 = static_cast<int>(p1Center[1]);
    int p2_1 = static_cast<int>(p2Center[1]);
    return ((p1_0 == p2_0) && (p1_1 == p2_1));
}


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
            if (moveDirection != MOVE_DIRECTION::DOWN)
            {
                moveDirection = MOVE_DIRECTION::UP;
            }

            break;
        case Key::Down:
            if (moveDirection != MOVE_DIRECTION::UP)
            {
                moveDirection = MOVE_DIRECTION::DOWN;
            }

            break;
        case Key::Right:
            if (moveDirection != MOVE_DIRECTION::LEFT)
            {
                moveDirection = MOVE_DIRECTION::RIGHT;
            }
            break;
        case Key::Left:
            if (moveDirection != MOVE_DIRECTION::RIGHT)
            {
                moveDirection = MOVE_DIRECTION::LEFT;
            }

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
    auto it = worm.begin();
    while (it != worm.end())
    {
        auto pieceModel = (*it)->getModel();
        auto center = (*it)->getCenter();
        it++;
        if (it != worm.end())
        {
            glm::vec3 nextPieceCenter = (*it)->getCenter();
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

std::shared_ptr<WormPiece> createRandomFood(std::shared_ptr<Layout> layout)
{
    float align = wormPieceSize;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine gen(seed);
    std::uniform_real_distribution<float> distribution(-coordinateSize, coordinateSize);
    float x = static_cast<float>(static_cast<int>(distribution(gen) / align)) * align;
    float y = static_cast<float>(static_cast<int>(distribution(gen) / align)) * align;
    auto foodShared = std::make_shared<WormPiece>(x - wormPieceSize / 2.0f, y + wormPieceSize / 2.0f, wormPieceVisible);
    layout->addElement(foodShared);
    return foodShared;
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

    std::shared_ptr<OrthographicCamera> m_camera = std::make_shared<OrthographicCamera>(-1000.0f * aspectRatio, 1000.0f * aspectRatio, -1000.0f, 1000.0f);
    std::vector<std::shared_ptr<Layout>> layouts = {};
    std::shared_ptr<Layout> wormLayout = std::make_shared<Layout>();
    Worm worm = {};
    initWorm(worm, wormLayout);
    std::weak_ptr<WormPiece> food = createRandomFood(wormLayout);
    layouts.push_back(wormLayout);


    time.getDelta<std::milli>();
    while (!shouldClose)
    {
        if (time.getDelta<std::milli>(false) >= frameInterval_ms)
        {
            time.getDelta<std::milli>();
            if (isTwoPiecesCollided(worm.back(), food.lock()))
            {
                worm.push_front(food.lock());
                food = createRandomFood(wormLayout);
                moveWorm(worm, wormPieceSize, moveDirection);
            }

            for (auto& l : layouts)
            {
                if (!shouldClose)
                {
                    l->draw(m_camera);
                    window->OnUpdate();
                }
            }


            moveWorm(worm, wormPieceSize, moveDirection);


        }
    }




    return 0;
}