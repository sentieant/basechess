#include "BaseChess/Piece/Pieces/Rook.hpp"
#include "BaseChess/ImplMove.hpp"

namespace Chess
{
    Tower::Tower(GridPoint pos, PlayerSide team, std::weak_ptr<GameBoard> field)
        : GamePiece(pos, PieceType::Tower, team, field)
    {}

    Tower::Tower(const GameEntity& entity, std::weak_ptr<GameBoard> field)
        : GamePiece(entity, field)
    {}

    Tower::Tower(const GamePiece& other)
        : GamePiece(other)
    {}

    std::shared_ptr<GamePiece> Tower::clonePiece(std::weak_ptr<GameBoard> field) const
    {
        return std::make_shared<Tower>(*this, field);
    }

    void Tower::computeMoves(std::vector<std::shared_ptr<GameMove>>& moveList, bool onlyCaptures) const
    {
        if (isCaptured) return;

        // Horizontal moves
        for (int i = pos.file + 1; i < 8; ++i)
            if (!validateMove(GridPoint(i, pos.rank), moveList)) break;

        for (int i = pos.file - 1; i >= 0; --i)
            if (!validateMove(GridPoint(i, pos.rank), moveList)) break;

        // Vertical moves
        for (int i = pos.rank + 1; i < 8; ++i)
            if (!validateMove(GridPoint(pos.file, i), moveList)) break;

        for (int i = pos.rank - 1; i >= 0; --i)
            if (!validateMove(GridPoint(pos.file, i), moveList)) break;
    }

    bool Tower::validateMove(const GridPoint& targetPos, std::vector<std::shared_ptr<GameMove>>& moveList) const
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