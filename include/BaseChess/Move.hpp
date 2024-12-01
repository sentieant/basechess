#pragma once

#include "BaseChess/Piece/Enums.hpp"
#include "BaseChess/Position.hpp"

#include <map>
#include <set>

namespace Chess
{
    class IGameMove
    {
    public:
        virtual const std::map<GridPoint, GridPoint>& getMoveSequence() const = 0;
        virtual const std::set<GridPoint>& getCapturePoints() const = 0;
        virtual const std::map<GridPoint, std::pair<PieceType, PieceType>>& getTransformations() const = 0;

        virtual int getMoveCount() const = 0;
    };

    typedef IGameMove GameMove;
}