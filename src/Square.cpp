#include "../include/Square.hpp"

short int Square::getCol() const
{
    return this->col - 1;
}

short int Square::getRow() const
{
    return 8 - this->row;
}

int Square::columnToInt(column colCode)
{
    return colCode - 1;
}

int Square::rowToInt(short int rowCode)
{
    return 8 - rowCode;
}

Square::Square(column _col, unsigned short int _row)
    : col(_col)
    , row(_row)
{
    if (this->col > 8 || this->col < 1 || this->row > 8 || this->row < 1)
        throw std::out_of_range(("Square which is (" + std::to_string(this->col) + "; " + std::to_string(this->row) + ") is out of the board, (1-8;1-8).").c_str());
}

bool Square::operator==(const Square other) const
{
    return this->col == other.col && this->row == other.row;
}
