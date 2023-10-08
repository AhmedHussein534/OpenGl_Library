#include <iostream>
#include <list>
#include <memory>
#include <chrono>
#include <windows.h>
#include <random>
#include <vector>
#include <array>
#include <unordered_map>

#include "SnakeGame.hpp"
#include "Debug.hpp"


namespace SnakeGame
{
    SnakeGame::SnakeGame() : Application("SnakeGame", coordinateSize, coordinateSize),
                             moveDirection(MOVE_DIRECTION::UP),
                             lastMoveDirection(MOVE_DIRECTION::UP)
    {
        initAssets();
        startGame();
    }

    void SnakeGame::startGame()
    {
        float aspectRatio = 1.0f;
        Worm worm = {};
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
        constexpr std::array<float, borderCount> xCoordinates = {-halfCoordinate,
                                                                -halfCoordinate,
                                                                -halfCoordinate,
                                                                halfCoordinate-wormPieceSize};
        constexpr std::array<float, borderCount> yCoordinates = {-halfCoordinate+wormPieceSize,
                                                                halfCoordinate,
                                                                halfCoordinate,
                                                                halfCoordinate};
        constexpr std::array<float, borderCount> length = {coordinateSize,
                                                        wormPieceSize,
                                                        coordinateSize,
                                                        wormPieceSize};
        constexpr std::array<float, borderCount> width = {wormPieceSize,
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
            resetFps();
            startGame();
            m_gameRunning = true;
        }

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



    void SnakeGame::initWorm()
    {
        HZ_PROFILE_FUNCTION();
        float currentXPiecePos = 0.0f - (wormPieceSize / 2.0f);
        for (uint32_t i = 0; i < wormLen; i++)
        {
            auto piece = std::make_shared<WormPiece>(getAsset("head_down"), currentXPiecePos, startYPos, wormPieceSize, wormPieceSize); //only draw 80% of each size to leave gap
            worm.push_back(piece);
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

    void SnakeGame::moveWormPieceInDirection(std::shared_ptr<WormPiece> piece)
    {
        HZ_PROFILE_FUNCTION();
        auto pieceModel = piece->getModel();
        auto dirUnitVector = getDirectionUnitVector(moveDirection);
        *pieceModel = glm::translate(*pieceModel, dirUnitVector * step);
    }

    void SnakeGame::moveWormPieceToOther(std::shared_ptr<WormPiece> pieceToMove, std::shared_ptr<WormPiece> otherPiece)
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

    void SnakeGame::assignAssetToHead(std::shared_ptr<WormPiece> head)
    {
        std::string dirStr = to_string(moveDirection);
        std::string headAsset = "head_" + dirStr;
        head->setTexAsset(getAsset(headAsset));
    }

    void SnakeGame::assignAssetToTail(std::shared_ptr<WormPiece> tail, std::shared_ptr<WormPiece> nextPiece)
    {
        auto direction = directionFromPieces(nextPiece, tail);
        std::string dirStr = to_string(direction);
        std::string tailAsset = "tail_" + dirStr;
        tail->setTexAsset(getAsset(tailAsset));
    }

    void SnakeGame::assignAssetToBody(std::shared_ptr<WormPiece> prev, std::shared_ptr<WormPiece> body, std::shared_ptr<WormPiece> next)
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

        body->setTexAsset(getAsset(bodyAsset));
    }

    void SnakeGame::moveWorm()
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
                moveWormPieceInDirection(pieceToMove);
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
                    assignAssetToTail(currentPiece, *it);
                }
                else
                {
                    assignAssetToBody(*prevItr, currentPiece, *it);
                }
            }
            else
            {
                assignAssetToHead(currentPiece);
            }
        }
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


    void SnakeGame::onDeltaStep()
    {
        if (isWormSelfCollided() || isPieceOutside(worm.back()))
        {
            std::cout << std::endl << std::endl << "Game over" << std::endl << std::endl;
            borders = {};
            worm.clear();
            food = nullptr;
            GL_ENGINE::Renderer2D::getRenderer().endScene();
            m_gameRunning = false;
        }
        else if (isTwoPiecesCollided(worm.back(), food))
        {
            auto headItr = worm.rbegin();
            auto prevHead = *(headItr);
            headItr++;
            auto beforePrevHead = *(headItr);
            moveWormPieceInDirection(food);
            assignAssetToHead(food);
            assignAssetToBody(beforePrevHead, prevHead, food);
            worm.push_back(food);
            food = createRandomFood();
            if (worm.size() % 5 == 0)
            {
                updateFps(getFps() * 1.1f);
            }

            std::cout << "\rScore: " << worm.size() - wormLen;
        }
        else
        {
            moveWorm();
        }

        GL_ENGINE::Renderer2D::getRenderer().drawScene();
        lastMoveDirection = moveDirection;
    }
}

