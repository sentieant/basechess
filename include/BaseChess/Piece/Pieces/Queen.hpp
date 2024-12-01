#pragma once

#include "BaseChess/Piece/MovablePiece.hpp"

namespace Chess
{
    class Regent : public GamePiece
    {
    public:
        Regent(GridPoint pos, PlayerSide team, std::weak_ptr<GameBoard> field);
        Regent(const GameEntity& entity, std::weak_ptr<GameBoard> field);
        Regent(const GamePiece& other);

        void computeMoves(std::vector<std::shared_ptr<GameMove>>& moveList, bool onlyCaptures = false) const override;
        std::shared_ptr<GamePiece> clonePiece(std::weak_ptr<GameBoard> field) const override;

    private:
        bool validateMove(const GridPoint& pos, std::vector<std::shared_ptr<GameMove>>& moveList) const;
    };
}