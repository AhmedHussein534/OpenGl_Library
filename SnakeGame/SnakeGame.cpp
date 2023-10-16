#include <iostream>
#include <chrono>
#include <windows.h>
#include <random>
#include <vector>
#include <unordered_map>

#include "SnakeGame.hpp"
#include "Debug.hpp"


namespace SnakeGame
{
    SnakeGame::SnakeGame() : Application("SnakeGame", coordinateSize, coordinateSize),
                             nextMoveDirection(MOVE_DIRECTION::RIGHT),
                             currentMoveDirection(MOVE_DIRECTION::RIGHT),
                             defaultFps(15.0f),
                             drawResolution(4),
                             resolutionStep(0)
    {
        initAssets();
        startGame();
    }

    void SnakeGame::startGame()
    {
        updateFps(defaultFps);
        resolutionStep = 0;
        float aspectRatio = 1.0f;
        GL_ENGINE::Renderer2D::getRenderer().beginScene(-halfCoordinate * aspectRatio, halfCoordinate * aspectRatio, -halfCoordinate, halfCoordinate);
        backgroundLayoutKey = GL_ENGINE::Renderer2D::getRenderer().createLayout();
        wormLayoutKey = GL_ENGINE::Renderer2D::getRenderer().createLayout();
        initBackground();
        initBorders();
        initWorm();
        food = createRandomFood();
    }

    std::string SnakeGame::to_string(MOVE_DIRECTION direction)
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

    bool SnakeGame::initAssets()
    {
        const std::string extenstion = ".png";
        std::array<std::string, 15> assetsToLoad = {
            "apple", "body_rightup", "body_leftup", "body_horizontal",
            "body_rightdown", "body_leftdown", "body_vertical", "head_down",
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

        texAssetsMap["body_downleft"]  = texAssetsMap["body_rightup"];
        texAssetsMap["body_upright"]   = texAssetsMap["body_leftdown"];
        texAssetsMap["body_upleft"]    = texAssetsMap["body_rightdown"];
        texAssetsMap["body_downright"] = texAssetsMap["body_leftup"];
        return true;
    }

    std::shared_ptr<GL_ENGINE::TextureAsset> SnakeGame::getAsset(const std::string &name)
    {
        auto pair = texAssetsMap.find(name);
        if (pair == texAssetsMap.end())
        {
            std::cout << "Error: asset is not in map: " << name << std::endl;
            assert(0);
        }

        return pair->second;
    }

    bool SnakeGame::initBorders()
    {
        HZ_PROFILE_FUNCTION();
        constexpr int32_t alignedHalfCoordinate = halfCoordinate - (halfCoordinate % wormPieceSize) + (wormPieceSize / 2);
        constexpr int32_t nAlignedHalfCoordinate = -alignedHalfCoordinate;

        constexpr std::array<int32_t, borderCount> xCoordinates = {nAlignedHalfCoordinate,
                                                                nAlignedHalfCoordinate,
                                                                nAlignedHalfCoordinate,
                                                                alignedHalfCoordinate-wormPieceSize};
        constexpr std::array<int32_t, borderCount> yCoordinates = {nAlignedHalfCoordinate+wormPieceSize,
                                                                alignedHalfCoordinate,
                                                                alignedHalfCoordinate,
                                                                alignedHalfCoordinate};
        constexpr std::array<int32_t, borderCount> length = {alignedHalfCoordinate * 2,
                                                        wormPieceSize,
                                                        alignedHalfCoordinate * 2,
                                                        wormPieceSize};
        constexpr std::array<int32_t, borderCount> width = {wormPieceSize,
                                                             alignedHalfCoordinate * 2,
                                                             wormPieceSize,
                                                             alignedHalfCoordinate * 2};
        size_t currentBorder = 0;
        for (auto &it : borders)
        {
            it = std::make_shared<border>(xCoordinates[currentBorder], yCoordinates[currentBorder], -0.4f, length[currentBorder], width[currentBorder], 0.34f, 0.54f, 0.20f, 1.0f);
            GL_ENGINE::Renderer2D::getRenderer().addElement(backgroundLayoutKey, it);
            currentBorder++;
        }

        return true;
    }

    bool SnakeGame::isTwoPiecesCollided(const std::shared_ptr<WormPiece> &p1, const std::shared_ptr<WormPiece> &p2)
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

    bool SnakeGame::isPieceOutside(const std::shared_ptr<WormPiece> piece)
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


    glm::vec3 SnakeGame::getDirectionUnitVector(MOVE_DIRECTION direction)
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


    bool SnakeGame::onCloseTriggered(const WindowCloseEvent &e)
    {
        std::cout << "onCloseTriggered" << std::endl;
        borders = {};
        worm.clear();
        food = nullptr;
        GL_ENGINE::Renderer2D::getRenderer().endScene();
        m_gameRunning = false;
        loopRunning = false;
        return true;
    }

    bool SnakeGame::onKeyPressed(const KeyPressedEvent &e)
    {
        if (!m_gameRunning)
        {
            m_gameRunning = true;
            startGame();
        }

        switch (e.GetKeyCode())
        {
            case Key::Up:
                if (currentMoveDirection != MOVE_DIRECTION::DOWN)
                {
                    nextMoveDirection = MOVE_DIRECTION::UP;
                }

                break;
            case Key::Down:
                if (currentMoveDirection != MOVE_DIRECTION::UP)
                {
                    nextMoveDirection = MOVE_DIRECTION::DOWN;
                }

                break;
            case Key::Right:
                if (currentMoveDirection != MOVE_DIRECTION::LEFT)
                {
                    nextMoveDirection = MOVE_DIRECTION::RIGHT;
                }
                break;
            case Key::Left:
                if (currentMoveDirection != MOVE_DIRECTION::RIGHT)
                {
                    nextMoveDirection = MOVE_DIRECTION::LEFT;
                }

                break;
            default:
                return true;
        }

        return true;
    }



    void SnakeGame::initWorm()
    {
        HZ_PROFILE_FUNCTION();
        constexpr float startYPos = wormPieceSize / 2.0f;
        float currentXPiecePos = 0.0f - (wormPieceSize / 2.0f);
        for (uint32_t i = 0; i < wormLen; i++)
        {
            auto piece = std::make_shared<WormPiece>(getAsset("head_down"), currentXPiecePos, startYPos, wormPieceSize, wormPieceSize); //only draw 80% of each size to leave gap
            worm.push_back(piece);
            directions.push_back(MOVE_DIRECTION::RIGHT);
            GL_ENGINE::Renderer2D::getRenderer().addElement(wormLayoutKey, piece);
            currentXPiecePos += wormPieceSize;
        }
    }

    void SnakeGame::initBackground()
    {
        HZ_PROFILE_FUNCTION();
        float red[2] = {170.0f/255.0f, 162.0f/255.0f};
        float green[2] = {215.0f/255.0f, 209.0f/255.0f};
        float blue[2] = {81.0f/255.0f, 73.0f/255.0f};


        int32_t startYPos = halfCoordinate - (halfCoordinate % wormPieceSize) + (wormPieceSize / 2);
        int32_t startXPos = -startYPos;
        int32_t currentXPos = startXPos;
        int32_t currentYPos = startYPos;

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

            currentYPos = startYPos;
            currentXPos += wormPieceSize;
            xColorIndex = (xColorIndex + 1) % 2;
        }
    }

    void SnakeGame::moveWormPieceInDirection(std::shared_ptr<WormPiece> piece, MOVE_DIRECTION direction, float m_step)
    {
        HZ_PROFILE_FUNCTION();
        auto pieceModel = piece->getModel();
        auto dirUnitVector = getDirectionUnitVector(direction);
        *pieceModel = glm::translate(*pieceModel, dirUnitVector * m_step);
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

    void SnakeGame::assignAssetToHead(std::shared_ptr<WormPiece> head)
    {
        std::string dirStr = to_string(*(directions.rbegin()));
        std::string headAsset = "head_" + dirStr;
        head->setTexAsset(getAsset(headAsset));
    }

    void SnakeGame::assignAssetToTail(std::shared_ptr<WormPiece> tail)
    {
        auto afterTail = directions.begin();
        afterTail++;

        std::string dirStr = to_string(*(afterTail));
        std::string tailAsset = "tail_" + dirStr;
        tail->setTexAsset(getAsset(tailAsset));
    }

    void SnakeGame::assignAssetToBody()
    {
        auto it = worm.begin();
        auto dirIt = directions.begin();
        it++; // skip tail
        while (true)
        {
            auto prevDir = *dirIt;
            auto currentPiece = it;
            it++;
            std::string bodyAsset = "body_";
            dirIt++;

            if (it != worm.end())
            {
                auto currentDir = *dirIt;
                auto nextDirIt = dirIt;
                nextDirIt++;
                auto nextDir = *nextDirIt;
                if ((currentDir == MOVE_DIRECTION::LEFT || currentDir == MOVE_DIRECTION::RIGHT) && (nextDir == MOVE_DIRECTION::LEFT || nextDir == MOVE_DIRECTION::RIGHT))
                {
                    bodyAsset += "horizontal";
                }
                else if ((currentDir == MOVE_DIRECTION::UP || currentDir == MOVE_DIRECTION::DOWN) && (nextDir == MOVE_DIRECTION::UP || nextDir == MOVE_DIRECTION::DOWN))
                {
                    bodyAsset += "vertical";
                }
                else
                {
                    std::string currentDirStr = to_string(currentDir);
                    bodyAsset += currentDirStr;
                    std::string nextDirStr = to_string(nextDir);
                    bodyAsset += nextDirStr;
                }
            }
            else
            {
                break;
            }

            (*currentPiece)->setTexAsset(getAsset(bodyAsset));
        }
    }

    void SnakeGame::moveWorm()
    {
        HZ_PROFILE_FUNCTION();
        auto drawStep = wormPieceSize / drawResolution;
        uint32_t dirItr = 0;
        auto it = worm.begin();
        auto dir = directions.begin();
        while (it != worm.end())
        {
            moveWormPieceInDirection(*it, *dir, drawStep);
            dir++;
            it++;
        }

        assignAssetsToWorm();

        GL_ENGINE::Renderer2D::getRenderer().drawScene();
    }

    void SnakeGame::assignAssetsToWorm()
    {
        assignAssetToHead(*(worm.rbegin()));
        assignAssetToTail(*(worm.begin()));
        assignAssetToBody();
    }


    std::shared_ptr<WormPiece> SnakeGame::createRandomFood()
    {
        HZ_PROFILE_FUNCTION();
        float align = wormPieceSize;
        uint32_t seed = static_cast<uint32_t>(std::chrono::system_clock::now().time_since_epoch().count());
        std::default_random_engine gen(seed);
        std::uniform_real_distribution<float> distribution(-halfCoordinate + wormPieceSize, halfCoordinate - wormPieceSize);
        float x = static_cast<float>(static_cast<int>(distribution(gen) / align)) * align;
        float y = static_cast<float>(static_cast<int>(distribution(gen) / align)) * align;
        auto foodShared = std::make_shared<WormPiece>(getAsset("apple"), x - wormPieceSize / 2.0f, y + wormPieceSize / 2.0f, wormPieceSize, wormPieceSize);
        GL_ENGINE::Renderer2D::getRenderer().addElement(wormLayoutKey, foodShared);
        return foodShared;
    }

    bool SnakeGame::isWormSelfCollided()
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

    bool SnakeGame::isInCoordinateCenter()
    {
        bool ret = false;
        if (resolutionStep % drawResolution == 0)
        {
            ret = true;
            resolutionStep = 0;
        }

        resolutionStep++;
        return ret;
    }

    void SnakeGame::updateMoveDirection()
    {
        currentMoveDirection = nextMoveDirection;
        directions.pop_front();
        directions.push_back(currentMoveDirection);
    }

    void SnakeGame::addPieceToWorm(std::shared_ptr<WormPiece> piece)
    {
        auto tailCenter = worm.front()->getCenter();
        auto tailDirection = directions.front();
        auto newCenter = tailCenter;
        switch(tailDirection)
        {
            case MOVE_DIRECTION::UP:
                newCenter[1] = newCenter[1] - wormPieceSize;
                break;
            case MOVE_DIRECTION::DOWN:
                newCenter[1] = newCenter[1] + wormPieceSize;
                break;
            case MOVE_DIRECTION::RIGHT:
                newCenter[0] = newCenter[0] - wormPieceSize;
                break;
            case MOVE_DIRECTION::LEFT:
                newCenter[0] = newCenter[0] + wormPieceSize;
                break;
            default:
                std::cout << "unknown direction" << std::endl;
                assert(0);
        }

         auto pieceModel = piece->getModel();
         auto center = piece->getCenter();
         *pieceModel = glm::translate(*pieceModel, newCenter - center);

         directions.push_front(directionFromPieces(piece, worm.front()));
         worm.push_front(piece);
    }

    void SnakeGame::onDeltaStep()
    {
        if (isWormSelfCollided() || isPieceOutside(worm.back()))
        {
            std::cout << std::endl << std::endl << "Game over" << std::endl << std::endl;
            m_gameRunning = false;
            borders = {};
            worm.clear();
            directions.clear();
            food = nullptr;
            GL_ENGINE::Renderer2D::getRenderer().endScene();

        }
        else
        {
            if (isInCoordinateCenter())
            {
                if (isTwoPiecesCollided(worm.back(), food))
                {
                    addPieceToWorm(food);
                    food = createRandomFood();
                    GL_ENGINE::Renderer2D::getRenderer().drawScene();
                }

                updateMoveDirection();
            }

            moveWorm();
        }
    }
}

