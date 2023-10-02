#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <list>
#include <memory>
#include <chrono>
#include <windows.h>
#include <random>
#include <vector>
#include <array>
#include <unordered_map>

#include "engine/Elements/Rectangle.hpp"
#include "engine/Elements/Texture.hpp"
#include "engine/Renderer2D.hpp"
#include "engine/Timestep.hpp"
#include "events/EventDispatcher.hpp"
#include "engine/Debug.hpp"

#include "window/Windows/WindowsWindow.hpp"


#define SWAP_INTERVAL 1


using WormPiece = GL_ENGINE::Texture;
using WormPiecePtr = std::shared_ptr<WormPiece>;
using Worm = std::list<std::shared_ptr<WormPiece>>;
using border = GL_ENGINE::Rectangle;
using AssetMap = std::unordered_map<std::string, std::shared_ptr<GL_ENGINE::TextureAsset>>;
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
const float wormPieceSize = 0.04f *  coordinateSize;// always make sure this is divisible by 2
const float startYPos = wormPieceSize / 2.0f;
float fps = 6;
MOVE_DIRECTION moveDirection = MOVE_DIRECTION::UP;
MOVE_DIRECTION lastMoveDirection = MOVE_DIRECTION::UP;
float aspectRatio = 0.0f;
constexpr size_t borderCount = 4;

std::string to_string(MOVE_DIRECTION direction)
{
    switch (direction)
    {
        case MOVE_DIRECTION::UP:
            return "up";
        case MOVE_DIRECTION::DOWN:
            return "down";
        case MOVE_DIRECTION::LEFT:
            return "left";
        case MOVE_DIRECTION::RIGHT:
            return "right";
        default:
            return "UnknownDir";
    }

    return "UnknownDir";
}

bool initAssets(AssetMap &texAssetsMap)
{
    const std::string extenstion = ".png";
    std::array<std::string, 15> assetsToLoad = {
        "apple", "body_downleft", "body_downright", "body_horizontal",
        "body_upleft", "body_upright", "body_vertical", "head_down",
        "head_left", "head_right", "head_up", "tail_down",
        "tail_left", "tail_right", "tail_up"};

    for (const auto& asset : assetsToLoad)
    {
        auto assetCreated = GL_ENGINE::StbTexture::getInstance().createTexAsset("res//" + asset + extenstion);
        if (!assetCreated)
        {
            std::cout << "ERROR: Failed to create asset: " << asset << std::endl;
            assert(0);
        }

        texAssetsMap[asset] = assetCreated;
    }

    texAssetsMap["body_rightup"] = texAssetsMap["body_downleft"];
    texAssetsMap["body_leftdown"] = texAssetsMap["body_upright"];
    texAssetsMap["body_rightdown"] = texAssetsMap["body_upleft"];
    texAssetsMap["body_leftup"] = texAssetsMap["body_downright"];
    return true;
}

std::shared_ptr<GL_ENGINE::TextureAsset> getAsset(AssetMap &texAssetsMap, const std::string &name)
{
    auto pair = texAssetsMap.find(name);
    if (pair == texAssetsMap.end())
    {
        std::cout << "Error: asset is not in map: " << name << std::endl;
        assert(0);
    }

    return pair->second;
}

bool initBorders(uint32_t backgroundLayoutKey, std::array<std::shared_ptr<border>, borderCount> borders)
{
    HZ_PROFILE_FUNCTION();
    std::array<float, borderCount> xCoordinates = {-halfCoordinate,
                                                   -halfCoordinate,
                                                   -halfCoordinate,
                                                    halfCoordinate-wormPieceSize};
    std::array<float, borderCount> yCoordinates = {-halfCoordinate+wormPieceSize,
                                                    halfCoordinate,
                                                    halfCoordinate,
                                                    halfCoordinate};
    std::array<float, borderCount> length = {coordinateSize,
                                             wormPieceSize,
                                             coordinateSize,
                                             wormPieceSize};
    std::array<float, borderCount> width = {wormPieceSize,
                                           coordinateSize,
                                           wormPieceSize,
                                           coordinateSize};
    size_t currentBorder = 0;
    for (auto &it : borders)
    {
        it = std::make_shared<border>(xCoordinates[currentBorder], yCoordinates[currentBorder], -0.4f, length[currentBorder], width[currentBorder], 0.34f, 0.54f, 0.20f, 1.0f);
        GL_ENGINE::Renderer2D::getRenderer().addElement(backgroundLayoutKey, it);
        currentBorder++;
    }

    return true;
}

bool isTwoPiecesCollided(const std::shared_ptr<WormPiece> &p1, const std::shared_ptr<WormPiece> &p2)
{
    HZ_PROFILE_FUNCTION();
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
    HZ_PROFILE_FUNCTION();
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
        ret = (squareX > static_cast<int>(halfCoordinate - wormPieceSize)) ||
              (squareX < static_cast<int>(-halfCoordinate + wormPieceSize)) ||
              (squareY < static_cast<int>(-halfCoordinate + wormPieceSize)) ||
              (squareY > static_cast<int>(halfCoordinate - wormPieceSize));
    }

    return ret;
}


glm::vec3 getDirectionUnitVector(MOVE_DIRECTION direction)
{
    HZ_PROFILE_FUNCTION();
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
    std::shared_ptr<WindowsWindow> window = std::make_shared<WindowsWindow>(WindowProps{"Snake", 1000, 1000});
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


void initWorm(uint32_t wormLayoutKey, Worm &worm, AssetMap& assetMap)
{
    HZ_PROFILE_FUNCTION();
    float currentXPiecePos = 0.0f - (wormPieceSize / 2.0f);
    for (uint32_t i = 0; i < wormLen; i++)
    {
        auto piece = std::make_shared<WormPiece>(getAsset(assetMap, "head_down"), currentXPiecePos, startYPos, wormPieceSize, wormPieceSize); //only draw 80% of each size to leave gap
        worm.push_back(piece);
        GL_ENGINE::Renderer2D::getRenderer().addElement(wormLayoutKey, piece);
        currentXPiecePos += wormPieceSize;
    }
}

void initBackground(uint32_t backgroundLayoutKey)
{
    HZ_PROFILE_FUNCTION();
    float red[2] = {170.0f/255.0f, 162.0f/255.0f};
    float green[2] = {215.0f/255.0f, 209.0f/255.0f};
    float blue[2] = {81.0f/255.0f, 73.0f/255.0f};

    float currentXPos = -halfCoordinate;
    float currentYPos = halfCoordinate;
    int xColorIndex = 0;
    while (currentXPos < halfCoordinate)
    {
        int yColorIndex = xColorIndex;
        while(currentYPos > -halfCoordinate)
        {
            auto rect = std::make_shared<GL_ENGINE::Rectangle>(currentXPos, currentYPos, -0.5f, wormPieceSize, wormPieceSize, red[yColorIndex], green[yColorIndex], blue[yColorIndex], 1.0f); //only draw 80% of each size to leave gap
            GL_ENGINE::Renderer2D::getRenderer().addElement(backgroundLayoutKey, rect);
            currentYPos -= wormPieceSize;
            yColorIndex = (yColorIndex + 1) % 2;
        }

        currentYPos = halfCoordinate;
        currentXPos += wormPieceSize;
        xColorIndex = (xColorIndex + 1) % 2;
    }
}

void moveWormPieceInDirection(std::shared_ptr<WormPiece> piece, float step = wormPieceSize, MOVE_DIRECTION direction = MOVE_DIRECTION::UP)
{
    HZ_PROFILE_FUNCTION();
    auto pieceModel = piece->getModel();
    auto dirUnitVector = getDirectionUnitVector(direction);
    *pieceModel = glm::translate(*pieceModel, dirUnitVector * step);
}

void moveWormPieceToOther(std::shared_ptr<WormPiece> pieceToMove, std::shared_ptr<WormPiece> otherPiece, float step = wormPieceSize)
{
    HZ_PROFILE_FUNCTION();
    auto pieceModel = pieceToMove->getModel();
    auto center = pieceToMove->getCenter();
    glm::vec3 nextPieceCenter = otherPiece->getCenter();
    *pieceModel = glm::translate(*pieceModel, glm::normalize(nextPieceCenter - center) * step);
}

MOVE_DIRECTION directionFromPieces(std::shared_ptr<WormPiece> piece, std::shared_ptr<WormPiece> next)
{
    MOVE_DIRECTION ret;
    auto pieceCenter = piece->getCenter();
    auto nextCenter = next->getCenter();
    if (nextCenter[0] > pieceCenter[0]) // X is greater is we move right
    {
        ret = MOVE_DIRECTION::RIGHT;
    }
    else if (nextCenter[0] < pieceCenter[0]) // x is smaller we move left
    {
        ret = MOVE_DIRECTION::LEFT;
    }
    else if (nextCenter[1] > pieceCenter[1]) // y is smaller we move top
    {
        ret = MOVE_DIRECTION::UP;
    }
    else
    {
        ret = MOVE_DIRECTION::DOWN;
    }

    return ret;
}

void assignAssetToHead(AssetMap &assetMap, std::shared_ptr<WormPiece> head, MOVE_DIRECTION direction)
{

    std::string dirStr = to_string(direction);
    std::string headAsset = "head_" + dirStr;
    head->setTexAsset(getAsset(assetMap, headAsset));
}

void assignAssetToTail(AssetMap &assetMap, std::shared_ptr<WormPiece> tail, std::shared_ptr<WormPiece> nextPiece)
{
    auto direction = directionFromPieces(nextPiece, tail);
    std::string dirStr = to_string(direction);
    std::string tailAsset = "tail_" + dirStr;
    tail->setTexAsset(getAsset(assetMap, tailAsset));
}

void assignAssetToBody(AssetMap &assetMap, std::shared_ptr<WormPiece> prev, std::shared_ptr<WormPiece> body, std::shared_ptr<WormPiece> next)
{
    std::string bodyAsset = "body_";
    auto prevDir = directionFromPieces(prev, body);
    auto nextDir = directionFromPieces(body, next);
    if ((prevDir == MOVE_DIRECTION::LEFT || prevDir == MOVE_DIRECTION::RIGHT) && (nextDir == MOVE_DIRECTION::LEFT || nextDir == MOVE_DIRECTION::RIGHT))
    {
        bodyAsset += "horizontal";
    }
    else if ((prevDir == MOVE_DIRECTION::UP || prevDir == MOVE_DIRECTION::DOWN) && (nextDir == MOVE_DIRECTION::UP || nextDir == MOVE_DIRECTION::DOWN))
    {
        bodyAsset += "vertical";
    }
    else
    {
        std::string prevDirStr = to_string(prevDir);
        bodyAsset += prevDirStr;
        auto nextDir = directionFromPieces(body, next);
        std::string nextDirStr = to_string(nextDir);
        bodyAsset += nextDirStr;
    }

    body->setTexAsset(getAsset(assetMap, bodyAsset));
}

void moveWorm(AssetMap &assetMap, Worm &worm, float step = wormPieceSize, MOVE_DIRECTION direction = MOVE_DIRECTION::UP)
{
    HZ_PROFILE_FUNCTION();
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

    it = worm.begin();
    while (it != worm.end())
    {
        bool isTail = (it == worm.begin());
        std::shared_ptr<WormPiece> currentPiece = *it;
        auto prevItr = it;
        if (it != worm.begin())
        {
            prevItr--;
        }

        it++;
        if (it != worm.end())
        {
            if (isTail)
            {
                assignAssetToTail(assetMap, currentPiece, *it);
            }
            else
            {
                assignAssetToBody(assetMap, *prevItr, currentPiece, *it);
            }
        }
        else
        {
            assignAssetToHead(assetMap, currentPiece, direction);
        }
    }



}

std::shared_ptr<WormPiece> createRandomFood(uint32_t wormLayout, AssetMap &assetMap)
{
    HZ_PROFILE_FUNCTION();
    float align = wormPieceSize;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine gen(seed);
    std::uniform_real_distribution<float> distribution(-halfCoordinate + wormPieceSize, halfCoordinate - wormPieceSize);
    float x = static_cast<float>(static_cast<int>(distribution(gen) / align)) * align;
    float y = static_cast<float>(static_cast<int>(distribution(gen) / align)) * align;
    auto foodShared = std::make_shared<WormPiece>(getAsset(assetMap, "apple"), x - wormPieceSize / 2.0f, y + wormPieceSize / 2.0f, wormPieceSize, wormPieceSize);
    GL_ENGINE::Renderer2D::getRenderer().addElement(wormLayout, foodShared);
    return foodShared;
}

bool isWormSelfCollided(Worm &worm)
{
    HZ_PROFILE_FUNCTION();
    bool isSelfCollided = false;
    auto it = worm.rbegin();
    auto head = it;
    it++;

    int i = 0;
    for (; it != worm.rend(); ++it)
    {
        if (isTwoPiecesCollided(*head, *it))
        {
            isSelfCollided = true;
            break;
        }
    }

    return isSelfCollided;

}


void executeGame(std::shared_ptr<WindowsWindow> window)
{
    {

        GL_ENGINE::Timestep time;
        AssetMap texAssetsMap;
        initAssets(texAssetsMap);
        auto step = wormPieceSize;

        GL_ENGINE::Renderer2D::getRenderer().beginScene(-halfCoordinate * aspectRatio, halfCoordinate * aspectRatio, -halfCoordinate, halfCoordinate);
        auto backgroundLayoutKey = GL_ENGINE::Renderer2D::getRenderer().createLayout();
        auto wormLayoutKey = GL_ENGINE::Renderer2D::getRenderer().createLayout();
        initBackground(backgroundLayoutKey);
        Worm worm = {};
        std::array<std::shared_ptr<border>, borderCount> borders = {};
        std::shared_ptr<WormPiece> food = createRandomFood(wormLayoutKey, texAssetsMap);
        initWorm(wormLayoutKey, worm, texAssetsMap);
        initBorders(backgroundLayoutKey, borders);

        std::cout << "Score: 0" << std::endl;
        float deltaTime = 1000.0f / fps;
        float sleepTime = 0.1f * deltaTime;
        while (!shouldClose)
        {
            auto delta = time.getDelta<std::milli>();
            if (delta >= deltaTime - sleepTime)
            {
                time.notifyUpdate();

                moveWorm(texAssetsMap, worm, step, moveDirection);

                if (isWormSelfCollided(worm) || isPieceOutside(worm.back()))
                {
                    std::cout << "Game over" << std::endl;
                    break;
                }

                if (isTwoPiecesCollided(worm.back(), food))
                {
                    std::cout << "Worm has eaten" << std::endl;
                    moveWormPieceInDirection(food, step, moveDirection);
                    worm.push_back(food);
                    food = createRandomFood(wormLayoutKey, texAssetsMap);
                    if (worm.size() % 5 == 0)
                    {
                        fps = fps * 1.1f;
                        deltaTime = 1000.0f / fps;
                        sleepTime = 0.1f * deltaTime;
                    }

                    std::cout << "Score: " << worm.size() - wormLen << std::endl;
                }

                if (!shouldClose)
                {
                    GL_ENGINE::Renderer2D::getRenderer().drawScene();
                    window->OnUpdate();
                }

                lastMoveDirection = moveDirection;
            }
            else
            {
                Sleep(sleepTime);
            }
        }

    }

    GL_ENGINE::Renderer2D::getRenderer().endScene();
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