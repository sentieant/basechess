#include "BaseChess/Piece/Pieces/Queen.hpp"
#include "BaseChess/ImplMove.hpp"

namespace Chess
{
    Regent::Regent(GridPoint pos, PlayerSide team, std::weak_ptr<GameBoard> field)
        : GamePiece(pos, PieceType::Regent, team, field)
    {}

    Regent::Regent(const GameEntity& entity, std::weak_ptr<GameBoard> field)
        : GamePiece(entity, field)
    {}

    Regent::Regent(const GamePiece& other)
        : GamePiece(other)
    {}

    std::shared_ptr<GamePiece> Regent::clonePiece(std::weak_ptr<GameBoard> field) const
    {
        return std::make_shared<Regent>(*this, field);
    }

    void Regent::computeMoves(std::vector<std::shared_ptr<GameMove>>& moveList, bool onlyCaptures) const
    {
        if (isCaptured) return;

        // Horizontal and vertical moves
        for (int i = pos.file + 1; i < 8; ++i)
            if (!validateMove(GridPoint(i, pos.rank), moveList)) break;

        for (int i = pos.file - 1; i >= 0; --i)
            if (!validateMove(GridPoint(i, pos.rank), moveList)) break;

        for (int i = pos.rank + 1; i < 8; ++i)
            if (!validateMove(GridPoint(pos.file, i), moveList)) break;

        for (int i = pos.rank - 1; i >= 0; --i)
            if (!validateMove(GridPoint(pos.file, i), moveList)) break;

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

    bool Regent::validateMove(const GridPoint& targetPos, std::vector<std::shared_ptr<GameMove>>& moveList) const
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