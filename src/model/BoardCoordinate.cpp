#include "../../include/model/BoardCoordinate.hpp"

short int BoardCoordinate::getCol() const
{
    return this->col;
}

short int BoardCoordinate::getRow() const
{
    return 8 - this->row;
}

int BoardCoordinate::columnToInt(Column colCode)
{
    return colCode;
}

int BoardCoordinate::rowToInt(short int rowCode)
{
    return 8 - rowCode;
}

BoardCoordinate::BoardCoordinate(Column _col, unsigned short int _row)
    : col(_col)
    , row(_row)
{
    if (this->col > 7 || this->col < 0 || this->row > 8 || this->row < 1)
        throw std::out_of_range(("Square which is (" + std::to_string(this->col) + "; " + std::to_string(this->row) + ") is out of the board, (0-7;1-8).").c_str());
}

bool BoardCoordinate::operator==(const BoardCoordinate other) const
{
    return this->col == other.col && this->row == other.row;
}
