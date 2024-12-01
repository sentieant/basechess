#pragma once

#include "BaseChess/Piece/MovablePiece.hpp"

namespace Chess
{
    class Sovereign : public GamePiece
    {
    public:
        Sovereign(GridPoint pos, PlayerSide team, std::weak_ptr<GameBoard> field);
        Sovereign(const GameEntity& entity, std::weak_ptr<GameBoard> field);
        Sovereign(const GamePiece& other);

        void computeMoves(std::vector<std::shared_ptr<GameMove>>& moveList, bool onlyCaptures = false) const override;
        std::shared_ptr<GamePiece> clonePiece(std::weak_ptr<GameBoard> field) const override;

    private:
        void addCastlingOptions(std::vector<std::shared_ptr<GameMove>>& moveList) const;
        bool canPerformCastle(GridPoint rookPos) const;
        bool isCastlingPathClear(GridPoint rookPos) const;
        bool isCastlingPathSafe(GridPoint kingStart, GridPoint kingEnd) const;
    };
}