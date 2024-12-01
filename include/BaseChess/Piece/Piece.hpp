#pragma once

#include "BaseChess/Position.hpp"
#include "BaseChess/Piece/Enums.hpp"
#include <memory>
#include <vector>

namespace Chess
{
    class GameEntity
    {
    public:
        GameEntity(GridPoint pos, PieceType type, PlayerSide team);
        GameEntity(const GameEntity& other);
        virtual ~GameEntity() = default;

        GridPoint pos;
        const PieceType type;
        const PlayerSide team;
        bool isCaptured;

    protected:
        std::vector<int> moveTicks;

    public:
        int getLastMoveTick() const;
        int getMoveCount() const;
        void incrementMoveCount(int tickCount);
        void decrementMoveCount();
    };
}