#pragma once
#include <iostream>

struct BoardCoordinate {
public:
    enum Column {
        A = 0,
        B = 1,
        C = 2,
        D = 3,
        E = 4,
        F = 5,
        G = 6,
        H = 7
    };

private:
    Column col;
    short int row;

public:
    short int getCol() const;
    short int getRow() const;
    static int columnToInt(Column colCode);
    static int rowToInt(short int rowCode);
    BoardCoordinate(Column _col, unsigned short int _row);
    bool operator==(const BoardCoordinate other) const;
};
