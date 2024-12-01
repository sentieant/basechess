#include "BaseChess/Piece/MovablePiece.hpp"

namespace Chess
{
    GamePiece::GamePiece(GridPoint pos, PieceType type, PlayerSide team, std::weak_ptr<GameBoard> field)
        : GameEntity(pos, type, team)
        , field(field)
    {}

    GamePiece::GamePiece(const GameEntity& entity, std::weak_ptr<GameBoard> field)
        : GameEntity(entity)
        , field(field)
    {}

    GamePiece::GamePiece(const GamePiece& other)
        : GameEntity(other)
        , field(other.field)
    {}
}