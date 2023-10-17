// SnakeGame.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <list>
#include <array>
#include <memory>

#include "Application.hpp"




namespace SnakeGame
{
    struct WormCoordinate
    {
        float x;
        float y;

        WormCoordinate(float m_x, float m_y) : x(m_x),
                                               y(m_y)
        {

        }

        glm::vec3 getCenter()
        {
            return glm::vec3(x, y, 0.0f);
        }

        void setCenter(const glm::vec3 &center)
        {
            x = center[0];
            y = center[1];
        }


    };

    using WormPiece = WormCoordinate;
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

    constexpr int32_t coordinateSize = 1000;
    constexpr int32_t halfCoordinate = coordinateSize / 2.0f;
    constexpr int32_t wormPieceSize = 0.04f *  coordinateSize;// always make sure this is divisible by 2
    constexpr int32_t wormLen = 4;
    constexpr size_t borderCount = 4;

    class SnakeGame : public GL_ENGINE::Application
    {
        public:
        SnakeGame();


        private:
        void startGame();
        void render();
        std::string to_string(MOVE_DIRECTION direction);
        bool initAssets();
        std::shared_ptr<GL_ENGINE::TextureAsset> getAsset(const std::string &name);
        bool initBorders();
        bool isTwoPiecesCollided(const std::shared_ptr<WormPiece> &p1, const std::shared_ptr<WormPiece> &p2);
        bool isPieceOutside(const std::shared_ptr<WormPiece> piece);
        glm::vec3 getDirectionUnitVector(MOVE_DIRECTION direction);
        void initWorm();
        void initBackground();
        void moveWormPieceInDirection(std::shared_ptr<WormPiece> piece, MOVE_DIRECTION direction, float m_step);
        void createHead();
        void createTail();
        void createBody();
        std::shared_ptr<WormPiece> createRandomFood();
        void moveWorm();
        bool isWormSelfCollided();
        void updateMoveDirection();
        bool isInCoordinateCenter();
        void addPieceToWorm(std::shared_ptr<WormPiece> piece);
        virtual void onDeltaStep() override;
        virtual bool onKeyPressed(const KeyPressedEvent &e) override;
        virtual bool onCloseTriggered(const WindowCloseEvent &e) override;


        private:
        AssetMap texAssetsMap;
        uint32_t backgroundLayoutKey;
        uint32_t wormLayoutKey;
        std::array<std::shared_ptr<border>, borderCount> borders;
        std::list<MOVE_DIRECTION> directions;
        std::shared_ptr<WormPiece> food;
        Worm worm;
        MOVE_DIRECTION nextMoveDirection;
        MOVE_DIRECTION currentMoveDirection;
        float defaultFps;
        uint32_t drawResolution;
        uint32_t resolutionStep;
    };
}
