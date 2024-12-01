#pragma once

#include "BaseChess/Position.hpp"
#include "BaseChess/Move.hpp"
#include "BaseChess/Piece/Piece.hpp"
#include "BaseChess/Piece/MovablePiece.hpp"
#include "BaseChess/Piece/Enums.hpp"

#include <memory>
#include <vector>

namespace Chess
{
    class GameBoard
    {
    public:
        virtual ~GameBoard() = default;

        virtual const std::shared_ptr<GameEntity> getPieceAt(GridPoint pos) const = 0;
        virtual const std::shared_ptr<GamePiece> getMovablePieceAt(GridPoint pos) const = 0;
        virtual std::vector<std::shared_ptr<GameEntity>> getPiecesByType(PieceType type, PlayerSide team) const = 0;
        
        virtual void computeMoves(std::vector<std::shared_ptr<GameMove>>& moveList, PlayerSide team, bool onlyCaptures = false) const = 0;
        virtual void executeMove(std::shared_ptr<GameMove> move) = 0;
        virtual void undoMove(std::shared_ptr<GameMove> move) = 0;
        
        virtual PlayerSide getActivePlayer() const = 0;
        virtual int getMoveCount() const = 0;
    };
}