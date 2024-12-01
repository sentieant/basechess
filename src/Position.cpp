#include "BaseChess/Position.hpp"

namespace Chess
{
    GridPoint::GridPoint()
        : file(-1)
        , rank(-1)
    {}

    GridPoint::GridPoint(int file, int rank)
        : file(file)
        , rank(rank)
    {}

    GridPoint::GridPoint(const GridPoint& other)
        : file(other.file)
        , rank(other.rank)
    {}

    bool GridPoint::operator==(const GridPoint& other) const
    {
        return file == other.file && rank == other.rank;
    }

    bool GridPoint::operator!=(const GridPoint& other) const
    {
        return !(*this == other);
    }

    bool GridPoint::operator<(const GridPoint& other) const
    {
        if (file != other.file)
            return file < other.file;
        return rank < other.rank;
    }

    bool GridPoint::isValid() const
    {
        return (file >= 0 && file <= 7 && rank >= 0 && rank <= 7);
    }
}