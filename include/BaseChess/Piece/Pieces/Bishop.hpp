#pragma once

#include "BaseChess/Piece/MovablePiece.hpp"

namespace Chess
{
    class Diagonal : public GamePiece
    {
    public:
        Diagonal(GridPoint pos, PlayerSide team, std::weak_ptr<GameBoard> field);
        Diagonal(const GameEntity& entity, std::weak_ptr<GameBoard> field);
        Diagonal(const GamePiece& other);

        void computeMoves(std::vector<std::shared_ptr<GameMove>>& moveList, bool onlyCaptures = false) const override;
        std::shared_ptr<GamePiece> clonePiece(std::weak_ptr<GameBoard> field) const override;

    private:
        bool validateMove(const GridPoint& pos, std::vector<std::shared_ptr<GameMove>>& moveList) const;
    };
}