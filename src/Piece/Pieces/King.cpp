#include "BaseChess/Piece/Pieces/King.hpp"
#include "BaseChess/ImplMove.hpp"

namespace Chess
{
    Sovereign::Sovereign(GridPoint pos, PlayerSide team, std::weak_ptr<GameBoard> field)
        : GamePiece(pos, PieceType::Sovereign, team, field)
    {}

    Sovereign::Sovereign(const GameEntity& entity, std::weak_ptr<GameBoard> field)
        : GamePiece(entity, field)
    {}

    Sovereign::Sovereign(const GamePiece& other)
        : GamePiece(other)
    {}

    std::shared_ptr<GamePiece> Sovereign::clonePiece(std::weak_ptr<GameBoard> field) const
    {
        return std::make_shared<Sovereign>(*this, field);
    }

    void Sovereign::computeMoves(std::vector<std::shared_ptr<GameMove>>& moveList, bool onlyCaptures) const
    {
        if (isCaptured) return;

        // Regular moves
        for (int i = -1; i <= 1; ++i)
        {
            for (int j = -1; j <= 1; ++j)
            {
                if (i == 0 && j == 0) continue;
                
                GridPoint targetPos(pos.file + i, pos.rank + j);
                if (!targetPos.isValid()) continue;

                std::shared_ptr<GameEntity> targetPiece = field.lock()->getPieceAt(targetPos);
                if (targetPiece && targetPiece->team == team) continue;

                std::shared_ptr<GameMoveImpl> move = std::make_shared<GameMoveImpl>(field);
                move->addMove(pos, targetPos);
                if (targetPiece) move->addCapture(targetPos);
                moveList.push_back(move);
            }
        }

        // Castling moves
        if (!onlyCaptures && getMoveCount() == 0)
            addCastlingOptions(moveList);
    }

    void Sovereign::addCastlingOptions(std::vector<std::shared_ptr<GameMove>>& moveList) const
    {
        // Kingside castling
        if (canPerformCastle(GridPoint(7, pos.rank)))
        {
            std::shared_ptr<GameMoveImpl> move = std::make_shared<GameMoveImpl>(field);
            move->addMove(pos, GridPoint(pos.file + 2, pos.rank));
            move->addMove(GridPoint(7, pos.rank), GridPoint(pos.file + 1, pos.rank));
            moveList.push_back(move);
        }

        // Queenside castling
        if (canPerformCastle(GridPoint(0, pos.rank)))
        {
            std::shared_ptr<GameMoveImpl> move = std::make_shared<GameMoveImpl>(field);
            move->addMove(pos, GridPoint(pos.file - 2, pos.rank));
            move->addMove(GridPoint(0, pos.rank), GridPoint(pos.file - 1, pos.rank));
            moveList.push_back(move);
        }
    }

    bool Sovereign::canPerformCastle(GridPoint rookPos) const
    {
        std::shared_ptr<GameEntity> rook = field.lock()->getPieceAt(rookPos);
        return rook && 
               rook->type == PieceType::Tower && 
               rook->team == team && 
               rook->getMoveCount() == 0 &&
               isCastlingPathClear(rookPos) &&
               isCastlingPathSafe(pos, GridPoint(rookPos.file > pos.file ? pos.file + 2 : pos.file - 2, pos.rank));
    }

    bool Sovereign::isCastlingPathClear(GridPoint rookPos) const
    {
        int direction = (rookPos.file > pos.file) ? 1 : -1;
        int distance = (rookPos.file > pos.file) ? rookPos.file - pos.file - 1 : pos.file - rookPos.file - 1;

        for (int i = 1; i <= distance; ++i)
        {
            if (field.lock()->getPieceAt(GridPoint(pos.file + i * direction, pos.rank)))
                return false;
        }
        return true;
    }

    bool Sovereign::isCastlingPathSafe(GridPoint kingStart, GridPoint kingEnd) const
    {
        std::vector<std::shared_ptr<GameMove>> enemyMoves;
        field.lock()->computeMoves(enemyMoves, 
            (team == PlayerSide::White) ? PlayerSide::Black : PlayerSide::White,
            true);

        int direction = (kingEnd.file > kingStart.file) ? 1 : -1;
        
        for (const auto& enemyMove : enemyMoves)
        {
            for (const auto& attackedPos : enemyMove->getCapturePoints())
            {
                if (attackedPos == kingStart) return false;
                
                for (int x = kingStart.file; x != kingEnd.file + direction; x += direction)
                {
                    if (attackedPos == GridPoint(x, kingStart.rank))
                        return false;
                }
            }
        }
        return true;
    }
}