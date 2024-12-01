#include "BaseChess/Piece/Pieces/Pawn.hpp"
#include "BaseChess/ImplMove.hpp"

namespace Chess
{
    Soldier::Soldier(GridPoint pos, PlayerSide team, std::weak_ptr<GameBoard> field)
        : GamePiece(pos, PieceType::Soldier, team, field)
    {}

    Soldier::Soldier(const GameEntity& entity, std::weak_ptr<GameBoard> field)
        : GamePiece(entity, field)
    {}

    Soldier::Soldier(const GamePiece& other)
        : GamePiece(other)
    {}

    std::shared_ptr<GamePiece> Soldier::clonePiece(std::weak_ptr<GameBoard> field) const
    {
        return std::make_shared<Soldier>(*this, field);
    }

    void Soldier::computeMoves(std::vector<std::shared_ptr<GameMove>>& moveList, bool onlyCaptures) const
    {
        if (isCaptured) return;

        if (!onlyCaptures)
            addAdvanceMoves(moveList);
        addCaptureMoves(moveList);
        addEnPassantMoves(moveList);
    }

    int Soldier::getAdvanceDirection() const
    {
        return (team == PlayerSide::White) ? 1 : -1;
    }

    void Soldier::addAdvanceMoves(std::vector<std::shared_ptr<GameMove>>& moveList) const
    {
        GridPoint oneStep(pos.file, pos.rank + getAdvanceDirection());
        if (!oneStep.isValid()) return;

        if (!field.lock()->getPieceAt(oneStep))
        {
            if (isPromotionRank(oneStep.rank))
            {
                std::shared_ptr<GameMoveImpl> move = std::make_shared<GameMoveImpl>(field);
                move->addMove(pos, oneStep);
                move->addTransformation(oneStep, PieceType::Soldier, PieceType::Regent);
                moveList.push_back(move);
            }
            else
            {
                std::shared_ptr<GameMoveImpl> move = std::make_shared<GameMoveImpl>(field);
                move->addMove(pos, oneStep);
                moveList.push_back(move);

                // Two-square advance from starting position
                if (((team == PlayerSide::White && pos.rank == 1) ||
                     (team == PlayerSide::Black && pos.rank == 6)))
                {
                    GridPoint twoStep(pos.file, pos.rank + 2 * getAdvanceDirection());
                    if (!field.lock()->getPieceAt(twoStep))
                    {
                        std::shared_ptr<GameMoveImpl> doubleMove = std::make_shared<GameMoveImpl>(field);
                        doubleMove->addMove(pos, twoStep);
                        moveList.push_back(doubleMove);
                    }
                }
            }
        }
    }

    void Soldier::addCaptureMoves(std::vector<std::shared_ptr<GameMove>>& moveList) const
    {
        for (int fileOffset = -1; fileOffset <= 1; fileOffset += 2)
        {
            GridPoint capturePos(pos.file + fileOffset, pos.rank + getAdvanceDirection());
            if (!capturePos.isValid()) continue;

            std::shared_ptr<GameEntity> targetPiece = field.lock()->getPieceAt(capturePos);
            if (targetPiece && targetPiece->team != team)
            {
                if (isPromotionRank(capturePos.rank))
                {
                    std::shared_ptr<GameMoveImpl> move = std::make_shared<GameMoveImpl>(field);
                    move->addMove(pos, capturePos);
                    move->addCapture(capturePos);
                    move->addTransformation(capturePos, PieceType::Soldier, PieceType::Regent);
                    moveList.push_back(move);
                }
                else
                {
                    std::shared_ptr<GameMoveImpl> move = std::make_shared<GameMoveImpl>(field);
                    move->addMove(pos, capturePos);
                    move->addCapture(capturePos);
                    moveList.push_back(move);
                }
            }
        }
    }

    void Soldier::addEnPassantMoves(std::vector<std::shared_ptr<GameMove>>& moveList) const
    {
        if ((team == PlayerSide::White && pos.rank != 4) ||
            (team == PlayerSide::Black && pos.rank != 3))
            return;

        for (int fileOffset = -1; fileOffset <= 1; fileOffset += 2)
        {
            GridPoint adjacentPos(pos.file + fileOffset, pos.rank);
            if (!adjacentPos.isValid()) continue;

            std::shared_ptr<GameEntity> adjacentPiece = field.lock()->getPieceAt(adjacentPos);
            if (adjacentPiece && 
                adjacentPiece->team != team && 
                adjacentPiece->type == PieceType::Soldier &&
                adjacentPiece->getMoveCount() == 1 &&
                adjacentPiece->getLastMoveTick() == field.lock()->getMoveCount() - 1)
            {
                GridPoint capturePos(adjacentPos.file, pos.rank + getAdvanceDirection());
                std::shared_ptr<GameMoveImpl> move = std::make_shared<GameMoveImpl>(field);
                move->addMove(pos, capturePos);
                move->addCapture(adjacentPos);
                moveList.push_back(move);
            }
        }
    }

    bool Soldier::isPromotionRank(int rank) const
    {
        return (team == PlayerSide::White && rank == 7) || 
               (team == PlayerSide::Black && rank == 0);
    }
}