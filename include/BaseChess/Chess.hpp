#pragma once

#include "BaseChess/Board.hpp"
#include "BaseChess/ImplBoard.hpp"
#include "BaseChess/Position.hpp"
#include "BaseChess/Move.hpp"
#include "BaseChess/Piece/Piece.hpp"
#include "BaseChess/Piece/MovablePiece.hpp"
#include "BaseChess/Piece/Enums.hpp"

#include <memory>
#include <vector>

namespace Chess
{
    class GameState
    {
    public:
        GameState(PieceType (*getPromotionTypeWhite)(), PieceType (*getPromotionTypeBlack)());
        GameState(const GameState& other);
        GameState& operator=(const GameState& other);

        const std::shared_ptr<GameEntity> getPieceAt(GridPoint pos) const;
        PlayerSide getGameResult() const;
        const std::shared_ptr<GameBoard> getGameBoard() const;
        bool executeMove(GridPoint source, GridPoint target);

    private:
        std::shared_ptr<GameBoardImpl> gameBoard;
        PlayerSide gameResult;
        PieceType (*getPromotionTypeWhite)();
        PieceType (*getPromotionTypeBlack)();

        bool validateMove(std::shared_ptr<GameMove> move);
    };
}