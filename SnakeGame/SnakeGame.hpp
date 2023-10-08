// SnakeGame.h : Include file for standard system include files,
// or project specific include files.

#pragma once

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

    constexpr uint32_t coordinateSize = 1000;
    constexpr float halfCoordinate = coordinateSize / 2.0f;
    constexpr float wormPieceSize = 0.04f *  coordinateSize;// always make sure this is divisible by 2
    constexpr float step = wormPieceSize;
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
        void moveWormPieceInDirection(std::shared_ptr<WormPiece> piece);
        void moveWormPieceToOther(std::shared_ptr<WormPiece> pieceToMove, std::shared_ptr<WormPiece> otherPiece);
        void assignAssetToHead(std::shared_ptr<WormPiece> head);
        void assignAssetToTail(std::shared_ptr<WormPiece> tail, std::shared_ptr<WormPiece> nextPiece);
        void assignAssetToBody(std::shared_ptr<WormPiece> prev, std::shared_ptr<WormPiece> body, std::shared_ptr<WormPiece> next);
        std::shared_ptr<WormPiece> createRandomFood();
        void moveWorm();
        bool isWormSelfCollided();
        void executeGame();
        virtual void onDeltaStep() override;
        virtual bool onKeyPressed(const KeyPressedEvent &e) override;
        virtual bool onCloseTriggered(const WindowCloseEvent &e) override;

        private:
        AssetMap texAssetsMap;
        uint32_t backgroundLayoutKey;
        uint32_t wormLayoutKey;
        std::array<std::shared_ptr<border>, borderCount> borders;
        std::shared_ptr<WormPiece> food;
        Worm worm;
        MOVE_DIRECTION moveDirection;
        MOVE_DIRECTION lastMoveDirection;
    };
}
