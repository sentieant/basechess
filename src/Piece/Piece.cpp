#include "BaseChess/Piece/Piece.hpp"

namespace Chess
{
    GameEntity::GameEntity(GridPoint pos, PieceType type, PlayerSide team)
        : pos(pos)
        , type(type)
        , team(team)
        , isCaptured(false)
    {}

    GameEntity::GameEntity(const GameEntity& other)
        : pos(other.pos)
        , type(other.type)
        , team(other.team)
        , isCaptured(other.isCaptured)
        , moveTicks(other.moveTicks)
    {}

    int GameEntity::getLastMoveTick() const
    {
        return moveTicks.empty() ? -1 : moveTicks.back();
    }

    int GameEntity::getMoveCount() const
    {
        return moveTicks.size();
    }

    void GameEntity::incrementMoveCount(int tickCount)
    {
        moveTicks.push_back(tickCount);
    }

    void GameEntity::decrementMoveCount()
    {
        if (!moveTicks.empty())
            moveTicks.pop_back();
    }
}