#pragma once

#include "BaseChess/Piece/Enums.hpp"
#include "BaseChess/Position.hpp"
#include "BaseChess/Board.hpp"
#include "BaseChess/Move.hpp"
#include "BaseChess/Piece/Piece.hpp"

#include <stdexcept>
#include <map>
#include <set>

namespace Chess
{
    class GameMoveImpl : public IGameMove
    {
    public:
        GameMoveImpl(std::weak_ptr<GameBoard> field);

        const std::map<GridPoint, GridPoint>& getMoveSequence() const override;
        const std::set<GridPoint>& getCapturePoints() const override;
        const std::map<GridPoint, std::pair<PieceType, PieceType>>& getTransformations() const override;

        int getMoveCount() const override;

        void addMove(GridPoint from, GridPoint to);
        void addCapture(GridPoint target);
        void addTransformation(GridPoint pos, PieceType oldType, PieceType newType);

    private:
        std::weak_ptr<GameBoard> field;
        int moveCount;

        std::map<GridPoint, GridPoint> moveSequence;
        std::set<GridPoint> capturePoints;
        std::map<GridPoint, std::pair<PieceType, PieceType>> transformations;
    };
}