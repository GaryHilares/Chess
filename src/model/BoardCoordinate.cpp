#include "../../include/model/BoardCoordinate.hpp"

short int BoardCoordinate::getCol() const
{
    return m_column;
}

short int BoardCoordinate::getRow() const
{
    return 8 - m_row;
}

int BoardCoordinate::columnToInt(Column column_code)
{
    return column_code;
}

int BoardCoordinate::rowToInt(short int row_code)
{
    return 8 - row_code;
}

BoardCoordinate::BoardCoordinate(Column column, unsigned short int row)
    : m_column(column)
    , m_row(row)
{
    if (m_column > 7 || m_column < 0 || m_row > 8 || m_row < 1) {
        throw std::out_of_range("Square which is (" + std::to_string(m_column) + "; " + std::to_string(m_row) + ") is out of the board, (0-7;1-8).");
    }
}

bool BoardCoordinate::operator==(const BoardCoordinate& other) const
{
    return m_column == other.m_column && m_row == other.m_row;
}
