#include "../../include/model/BoardCoordinate.hpp"

int BoardCoordinate::getCol() const
{
    return m_column;
}

int BoardCoordinate::getRow() const
{
    return m_row;
}

BoardCoordinate::BoardCoordinate(int column, int row)
    : m_column(column)
    , m_row(row)
{
    if (m_column > 8 || m_column < 1 || m_row > 8 || m_row < 1) {
        throw std::out_of_range("Square which is (" + std::to_string(m_column) + "; " + std::to_string(m_row) + ") is out of the board, (1-8;1-8).");
    }
}

bool BoardCoordinate::operator==(const BoardCoordinate& that)
{
    return this->m_column == that.m_column && this->m_row == that.m_row;
}
