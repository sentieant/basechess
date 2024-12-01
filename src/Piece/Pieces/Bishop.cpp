#include "BaseChess/Piece/Pieces/Bishop.hpp"
#include "BaseChess/ImplMove.hpp"

namespace Chess
{
    Diagonal::Diagonal(GridPoint pos, PlayerSide team, std::weak_ptr<GameBoard> field)
        : GamePiece(pos, PieceType::Diagonal, team, field)
    {}

    Diagonal::Diagonal(const GameEntity& entity, std::weak_ptr<GameBoard> field)
        : GamePiece(entity, field)
    {}

    Diagonal::Diagonal(const GamePiece& other)
        : GamePiece(other)
    {}

    std::shared_ptr<GamePiece> Diagonal::clonePiece(std::weak_ptr<GameBoard> field) const
    {
        return std::make_shared<Diagonal>(*this, field);
    }

    void Diagonal::computeMoves(std::vector<std::shared_ptr<GameMove>>& moveList, bool onlyCaptures) const
    {
        if (isCaptured) return;

        // Diagonal moves
        for (int i = 1; pos.file + i < 8 && pos.rank + i < 8; ++i)
            if (!validateMove(GridPoint(pos.file + i, pos.rank + i), moveList)) break;

        for (int i = 1; pos.file + i < 8 && pos.rank - i >= 0; ++i)
            if (!validateMove(GridPoint(pos.file + i, pos.rank - i), moveList)) break;

        for (int i = 1; pos.file - i >= 0 && pos.rank + i < 8; ++i)
            if (!validateMove(GridPoint(pos.file - i, pos.rank + i), moveList)) break;

        for (int i = 1; pos.file - i >= 0 && pos.rank - i >= 0; ++i)
            if (!validateMove(GridPoint(pos.file - i, pos.rank - i), moveList)) break;
    }

    bool Diagonal::validateMove(const GridPoint& targetPos, std::vector<std::shared_ptr<GameMove>>& moveList) const
    {
        if (!targetPos.isValid()) return false;

        std::shared_ptr<GameEntity> targetPiece = field.lock()->getPieceAt(targetPos);
        
        if (targetPiece)
        {
            if (targetPiece->team == team)
                return false;
                
            std::shared_ptr<GameMoveImpl> move = std::make_shared<GameMoveImpl>(field);
            move->addMove(pos, targetPos);
            move->addCapture(targetPos);
            moveList.push_back(move);
            return false;
        }

        std::shared_ptr<GameMoveImpl> move = std::make_shared<GameMoveImpl>(field);
        move->addMove(pos, targetPos);
        move->addCapture(targetPos);
        moveList.push_back(move);
        return true;
    }
}