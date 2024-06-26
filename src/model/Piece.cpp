#include "../../include/model/Piece.hpp"
#include <cassert>

Piece::Piece(PieceColor color, PieceType type)
    : m_color(color)
    , m_type(type)
    , m_has_moved(false)
{
}

void Piece::setAsMoved()
{
    m_has_moved = true;
}

bool Piece::hasMoved() const
{
    return m_has_moved;
}

void Piece::promotePawnTo(PieceType to_promote)
{
    assert(m_type == PieceType::Pawn);
    m_type = to_promote;
}

PieceColor Piece::getColor() const
{
    return m_color;
}

PieceType Piece::getType() const
{
    return m_type;
}

bool Piece::operator==(const Piece& that) const
{
    return m_color == that.m_color
        && m_type == that.m_type
        && m_has_moved == that.m_has_moved;
}

bool Piece::operator!=(const Piece& that) const
{
    return !(*this == that);
}