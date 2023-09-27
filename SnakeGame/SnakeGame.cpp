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
#include <array>

#include "engine/Layout.hpp"
#include "engine/Elements/Rectangle.hpp"
#include "engine/Cameras/OrthographicCamera.hpp"
#include "engine/Timestep.hpp"
#include "events/EventDispatcher.hpp"


#include "window/Windows/WindowsWindow.hpp"


#define SWAP_INTERVAL 1


using WormPiece = GL_ENGINE::Rectangle;
using WormPiecePtr = std::shared_ptr<WormPiece>;
using Worm = std::list<std::shared_ptr<WormPiece>>;
using border = GL_ENGINE::Rectangle;

enum class MOVE_DIRECTION : uint32_t
{
    UP,
    DOWN,
    RIGHT,
    LEFT,
};


bool shouldClose = false;
const float coordinateSize = 1000.0f;
const float halfCoordinate = coordinateSize / 2.0f;
const uint32_t wormLen = 4;
const float wormPieceSize = 0.02f *  coordinateSize;// always make sure this is divisible by 2
const float wormPieceVisible = wormPieceSize * 0.8f;
const float startYPos = wormPieceSize / 2.0f;
float fps = 6;
MOVE_DIRECTION moveDirection = MOVE_DIRECTION::UP;
MOVE_DIRECTION lastMoveDirection = MOVE_DIRECTION::UP;
float aspectRatio = 0.0f;
constexpr size_t borderCount = 4;

bool initBorders(std::shared_ptr<GL_ENGINE::Layout> layout, std::array<std::shared_ptr<border>, borderCount> borders)
{
    std::array<float, borderCount> xCoordinates = {-halfCoordinate                , -halfCoordinate , -halfCoordinate,  halfCoordinate - wormPieceVisible};
    std::array<float, borderCount> yCoordinates = {wormPieceVisible - halfCoordinate,  halfCoordinate,   halfCoordinate,  halfCoordinate};
    std::array<float, borderCount> length = {coordinateSize, wormPieceVisible, coordinateSize, wormPieceVisible};
    std::array<float, borderCount> width = {wormPieceVisible,  coordinateSize, wormPieceVisible, coordinateSize};
    size_t currentBorder = 0;
    for (auto &it : borders)
    {
        it = std::make_shared<border>(xCoordinates[currentBorder], yCoordinates[currentBorder], length[currentBorder], width[currentBorder]);
        layout->addElement(it);
        currentBorder++;
    }
}

bool isTwoPiecesCollided(const std::shared_ptr<WormPiece> p1, const std::shared_ptr<WormPiece> p2)
{
    bool ret = false;
    if ((p1 == nullptr) || (p2 == nullptr))
    {
        ret = false;
    }
    else
    {
        auto p1Center = p1->getCenter();
        auto p2Center = p2->getCenter();
        int p1_0 = static_cast<int>(p1Center[0]);
        int p2_0 = static_cast<int>(p2Center[0]);
        int p1_1 = static_cast<int>(p1Center[1]);
        int p2_1 = static_cast<int>(p2Center[1]);
        ret =  ((p1_0 == p2_0) && (p1_1 == p2_1));
    }

    return ret;
}

bool isPieceOutside(const std::shared_ptr<WormPiece> piece)
{
    bool ret = false;
    if ((piece == nullptr))
    {
        ret = false;
    }
    else
    {
        auto squareCenter = piece->getCenter();
        int squareX = static_cast<int>(squareCenter[0]);
        int squareY = static_cast<int>(squareCenter[1]);
        ret = (squareX > static_cast<int>(halfCoordinate - wormPieceVisible)) ||
              (squareX < static_cast<int>(-halfCoordinate + wormPieceVisible)) ||
              (squareY < static_cast<int>(-halfCoordinate + wormPieceVisible)) ||
              (squareY > static_cast<int>(halfCoordinate - wormPieceVisible));
    }

    return ret;
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
    std::shared_ptr<WindowsWindow> window = std::make_shared<WindowsWindow>(WindowProps{"Mywindow", 1000, 1000});
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
            if (lastMoveDirection != MOVE_DIRECTION::DOWN)
            {
                moveDirection = MOVE_DIRECTION::UP;
            }

            break;
        case Key::Down:
            if (lastMoveDirection != MOVE_DIRECTION::UP)
            {
                moveDirection = MOVE_DIRECTION::DOWN;
            }

            break;
        case Key::Right:
            if (lastMoveDirection != MOVE_DIRECTION::LEFT)
            {
                moveDirection = MOVE_DIRECTION::RIGHT;
            }
            break;
        case Key::Left:
            if (lastMoveDirection != MOVE_DIRECTION::RIGHT)
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
    EventDispatcher::getInstance().Dispatch<KeyReleasedEvent>(e, onKeyReleased);
    EventDispatcher::getInstance().Dispatch<WindowCloseEvent>(e, onCloseTriggered);
}


void initWorm(Worm &worm,  std::shared_ptr<GL_ENGINE::Layout> layout)
{
    float currentXPiecePos = 0.0f - (wormPieceSize / 2.0f);
    for (uint32_t i = 0; i < wormLen; i++)
    {
        auto piece = std::make_shared<WormPiece>(currentXPiecePos, startYPos, wormPieceVisible, wormPieceVisible); //only draw 80% of each size to leave gap
        worm.push_back(piece);
        layout->addElement(piece);
        currentXPiecePos += wormPieceSize;
    }

}

void moveWormPieceInDirection(std::shared_ptr<WormPiece> piece, float step = wormPieceSize, MOVE_DIRECTION direction = MOVE_DIRECTION::UP)
{
    auto pieceModel = piece->getModel();
    auto dirUnitVector = getDirectionUnitVector(direction);
    *pieceModel = glm::translate(*pieceModel, dirUnitVector * step);
}

void moveWormPieceToOther(std::shared_ptr<WormPiece> pieceToMove, std::shared_ptr<WormPiece> otherPiece, float step = wormPieceSize)
{
    auto pieceModel = pieceToMove->getModel();
    auto center = pieceToMove->getCenter();
    glm::vec3 nextPieceCenter = otherPiece->getCenter();
    *pieceModel = glm::translate(*pieceModel, glm::normalize(nextPieceCenter - center) * step);
}

void moveWorm(Worm &worm, float step = wormPieceSize, MOVE_DIRECTION direction = MOVE_DIRECTION::UP)
{
    auto it = worm.begin();
    while (it != worm.end())
    {
        std::shared_ptr<WormPiece> pieceToMove = *it;
        it++;
        if (it != worm.end())
        {
            moveWormPieceToOther(pieceToMove, *it);
        }
        else
        {
            moveWormPieceInDirection(pieceToMove, step, direction);
        }
    }

}

std::shared_ptr<WormPiece> createRandomFood(std::shared_ptr<GL_ENGINE::Layout> layout)
{
    float align = wormPieceSize;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine gen(seed);
    std::uniform_real_distribution<float> distribution(-halfCoordinate + wormPieceSize, halfCoordinate - wormPieceSize);
    float x = static_cast<float>(static_cast<int>(distribution(gen) / align)) * align;
    float y = static_cast<float>(static_cast<int>(distribution(gen) / align)) * align;
    auto foodShared = std::make_shared<WormPiece>(x - wormPieceSize / 2.0f, y + wormPieceSize / 2.0f, wormPieceVisible, wormPieceVisible, 255.0f, 0.0f, 0.0f);
    layout->addElement(foodShared);
    return foodShared;
}

bool isWormSelfCollided(Worm &worm)
{
    bool isSelfCollided = false;
    auto it = worm.rbegin();
    auto head = it;
    it++;
    for (; it != worm.rend(); ++it)
    {
        if (isTwoPiecesCollided(*head, *it))
        {
            isSelfCollided = true;
        }
    }

    return isSelfCollided;

}


void executeGame(std::shared_ptr<WindowsWindow> window)
{
    GL_ENGINE::Timestep time;
    auto step = wormPieceSize;
    std::shared_ptr<GL_ENGINE::OrthographicCamera> m_camera = std::make_shared<GL_ENGINE::OrthographicCamera>(-halfCoordinate * aspectRatio, halfCoordinate * aspectRatio, -halfCoordinate, halfCoordinate);
    std::vector<std::shared_ptr<GL_ENGINE::Layout>> layouts = {};
    std::shared_ptr<GL_ENGINE::Layout> wormLayout = std::make_shared<GL_ENGINE::Layout>();
    Worm worm = {};
    std::array<std::shared_ptr<border>, borderCount> borders = {};
    std::weak_ptr<WormPiece> food = createRandomFood(wormLayout);

    layouts.push_back(wormLayout);

    initWorm(worm, wormLayout);
    initBorders(wormLayout, borders);
    std::cout << "Score: 0" << std::endl;

    while (!shouldClose)
    {
        auto delta = time.getDelta<std::milli>();
        if (delta >= (1000.0f / fps))
        {
            time.notifyUpdate();
            if (isTwoPiecesCollided(worm.back(), food.lock()))
            {
                moveWormPieceInDirection(food.lock(), step, moveDirection);
                worm.push_back(food.lock());
                food = createRandomFood(wormLayout);
                if (worm.size() % 5 == 0)
                {
                    fps = fps * 1.1f;
                }

                std::cout << "Score: " << worm.size() - wormLen << std::endl;
            }

            for (auto& l : layouts)
            {
                if (!shouldClose)
                {
                    l->draw(m_camera);
                    window->OnUpdate();
                }
            }

            lastMoveDirection = moveDirection;
            moveWorm(worm, step, moveDirection);
            if (isWormSelfCollided(worm))
            {
                break;
            }

            if (isPieceOutside(worm.back()))
            {
                break;
            }
        }
        else
        {
            sleep((1000.0f / fps)/10000.0);
        }
    }
}

int main(void)
{

    EventDispatcher::getInstance().subscribeToEvents(EventCategoryKeyboard | EventCategoryApplication, onEvent);
    auto window = init();
    if (window == nullptr)
    {
        return -1;
    }

    aspectRatio = static_cast<float>(window->GetWidth()) / static_cast<float>(window->GetHeight());
    executeGame(window);
    return 0;
}