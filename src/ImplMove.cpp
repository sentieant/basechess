#include "BaseChess/ImplMove.hpp"

namespace Chess
{
    GameMoveImpl::GameMoveImpl(std::weak_ptr<GameBoard> field)
        : field(field)
        , moveCount(0)
    {}

    const std::map<GridPoint, GridPoint>& GameMoveImpl::getMoveSequence() const
    {
        return moveSequence;
    }

    const std::set<GridPoint>& GameMoveImpl::getCapturePoints() const
    {
        return capturePoints;
    }

    const std::map<GridPoint, std::pair<PieceType, PieceType>>& GameMoveImpl::getTransformations() const
    {
        return transformations;
    }

    int GameMoveImpl::getMoveCount() const
    {
        return moveCount;
    }

    void GameMoveImpl::addMove(GridPoint from, GridPoint to)
    {
        moveSequence[from] = to;
        moveCount++;
    }

    void GameMoveImpl::addCapture(GridPoint target)
    {
        capturePoints.insert(target);
    }

    void GameMoveImpl::addTransformation(GridPoint pos, PieceType oldType, PieceType newType)
    {
        transformations[pos] = std::make_pair(oldType, newType);
    }
}