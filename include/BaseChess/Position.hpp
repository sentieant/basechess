#pragma once

namespace Chess
{
    struct GridPoint
    {
        int file;  // horizontal position
        int rank;  // vertical position

        GridPoint();
        GridPoint(int file, int rank);
        GridPoint(const GridPoint& other);

        bool operator==(const GridPoint& other) const;
        bool operator!=(const GridPoint& other) const;
        bool operator<(const GridPoint& other) const;

        bool isValid() const;
    };
}