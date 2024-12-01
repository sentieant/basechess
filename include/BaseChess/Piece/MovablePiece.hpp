#pragma once

#include "BaseChess/Piece/Piece.hpp"
#include "BaseChess/Move.hpp"
#include <vector>
#include <memory>

namespace Chess
{
    class GameBoard;

    class GamePiece : public GameEntity
    {
    public:
        GamePiece(GridPoint pos, PieceType type, PlayerSide team, std::weak_ptr<GameBoard> field);
        GamePiece(const GameEntity& entity, std::weak_ptr<GameBoard> field);
        GamePiece(const GamePiece& other);
        virtual ~GamePiece() = default;

        virtual void computeMoves(std::vector<std::shared_ptr<GameMove>>& moveList, bool onlyCaptures = false) const = 0;
        virtual std::shared_ptr<GamePiece> clonePiece(std::weak_ptr<GameBoard> field) const = 0;

    protected:
        std::weak_ptr<GameBoard> field;
    };
}