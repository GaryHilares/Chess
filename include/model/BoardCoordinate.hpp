#pragma once
#include <iostream>

namespace BoardColumn {
    constexpr int A = 1;
    constexpr int B = 2;
    constexpr int C = 3;
    constexpr int D = 4;
    constexpr int E = 5;
    constexpr int F = 6;
    constexpr int G = 7;
    constexpr int H = 8;
}

struct BoardCoordinate {
private:
    int m_column;
    int m_row;

public:
    BoardCoordinate(int column, int row);
    int getCol() const;
    int getRow() const;
    bool operator==(const BoardCoordinate& that);
};
