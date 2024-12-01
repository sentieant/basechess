#pragma once

#include "BaseChess/Piece/MovablePiece.hpp"

namespace Chess
{
    class Soldier : public GamePiece
    {
    public:
        Soldier(GridPoint pos, PlayerSide team, std::weak_ptr<GameBoard> field);
        Soldier(const GameEntity& entity, std::weak_ptr<GameBoard> field);
        Soldier(const GamePiece& other);

        void computeMoves(std::vector<std::shared_ptr<GameMove>>& moveList, bool onlyCaptures = false) const override;
        std::shared_ptr<GamePiece> clonePiece(std::weak_ptr<GameBoard> field) const override;

    private:
        int getAdvanceDirection() const;
        void addAdvanceMoves(std::vector<std::shared_ptr<GameMove>>& moveList) const;
        void addCaptureMoves(std::vector<std::shared_ptr<GameMove>>& moveList) const;
        void addEnPassantMoves(std::vector<std::shared_ptr<GameMove>>& moveList) const;
        bool isPromotionRank(int rank) const;
    };
}