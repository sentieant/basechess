#include "BaseChess/Piece/Pieces/Knight.hpp"
#include "BaseChess/ImplMove.hpp"

namespace Chess
{
    Steed::Steed(GridPoint pos, PlayerSide team, std::weak_ptr<GameBoard> field)
        : GamePiece(pos, PieceType::Steed, team, field)
    {}

    Steed::Steed(const GameEntity& entity, std::weak_ptr<GameBoard> field)
        : GamePiece(entity, field)
    {}

    Steed::Steed(const GamePiece& other)
        : GamePiece(other)
    {}

    std::shared_ptr<GamePiece> Steed::clonePiece(std::weak_ptr<GameBoard> field) const
    {
        return std::make_shared<Steed>(*this, field);
    }

    void Steed::computeMoves(std::vector<std::shared_ptr<GameMove>>& moveList, bool onlyCaptures) const
    {
        if (isCaptured) return;

        validateMove(GridPoint(pos.file + 1, pos.rank + 2), moveList);
        validateMove(GridPoint(pos.file + 2, pos.rank + 1), moveList);
        validateMove(GridPoint(pos.file + 2, pos.rank - 1), moveList);
        validateMove(GridPoint(pos.file + 1, pos.rank - 2), moveList);
        validateMove(GridPoint(pos.file - 1, pos.rank - 2), moveList);
        validateMove(GridPoint(pos.file - 2, pos.rank - 1), moveList);
        validateMove(GridPoint(pos.file - 2, pos.rank + 1), moveList);
        validateMove(GridPoint(pos.file - 1, pos.rank + 2), moveList);
    }

    bool Steed::validateMove(const GridPoint& pos, std::vector<std::shared_ptr<GameMove>>& moveList) const
    {
        if (!pos.isValid()) return false;

        std::shared_ptr<GameEntity> targetPiece = field.lock()->getPieceAt(pos);
        
        if (targetPiece && targetPiece->team == team)
            return false;

        std::shared_ptr<GameMoveImpl> move = std::make_shared<GameMoveImpl>(field);
        move->addMove(this->pos, pos);
        move->addCapture(pos);
        moveList.push_back(move);
        return true;
    }
}