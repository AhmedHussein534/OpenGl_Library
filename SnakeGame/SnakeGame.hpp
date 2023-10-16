// SnakeGame.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <list>
#include <array>
#include <memory>

#include "Application.hpp"




namespace SnakeGame
{
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

    struct PieceToAdd
    {
        std::shared_ptr<WormPiece> piece;
        uint32_t countDown;

        PieceToAdd(std::shared_ptr<WormPiece> m_piece, uint32_t m_countDown) : piece(m_piece),
                                                                               countDown(m_countDown)
        {

        }
    };

    constexpr uint32_t coordinateSize = 1000;
    constexpr float halfCoordinate = coordinateSize / 2.0f;
    constexpr float wormPieceSize = 0.04f *  coordinateSize;// always make sure this is divisible by 2
    constexpr float startYPos = wormPieceSize / 2.0f;
    constexpr uint32_t wormLen = 4;
    constexpr size_t borderCount = 4;

    class SnakeGame : public GL_ENGINE::Application
    {
        public:
        SnakeGame();


        private:
        void startGame();
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
        void assignAssetToHead(std::shared_ptr<WormPiece> head);
        void assignAssetToTail(std::shared_ptr<WormPiece> tail);
        void assignAssetToBody();
        std::shared_ptr<WormPiece> createRandomFood();
        void moveWorm();
        bool isWormSelfCollided();
        void assignAssetsToWorm();
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
        std::list<PieceToAdd> piecesToAdd;
        Worm worm;
        MOVE_DIRECTION nextMoveDirection;
        MOVE_DIRECTION currentMoveDirection;
        float defaultFps;
        uint32_t drawResolution;
        uint32_t resolutionStep;
    };
}
