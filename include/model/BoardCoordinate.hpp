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
    Column m_column;
    short int m_row;

public:
    BoardCoordinate(Column column, unsigned short int row);
    short int getCol() const;
    short int getRow() const;
    static int columnToInt(Column column_code);
    static int rowToInt(short int row_code);
    bool operator==(const BoardCoordinate& other) const;
};
