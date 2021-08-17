#pragma once
#include <iostream>

struct square
{
public:
    enum column{a = 1, b = 2, c = 3, d = 4, e = 5, f = 6, g = 7, h = 8};
private:
    column col;
    short int row;
public:
    short int getCol() const;
    short int getRow() const;
    static int columnToInt(column colCode);
    static int rowToInt(short int rowCode);
    square(column _col, unsigned short int _row);
    bool operator==(const square other) const;
};
