#pragma once
#include "PieceColor.hpp"
#include "PieceType.hpp"

class Piece {
private:
    PieceColor m_color;
    PieceType m_type;
    bool m_has_moved;

public:
    Piece(PieceColor color, PieceType type);
    void setAsMoved();
    bool hasMoved() const;
    void promotePawnTo(PieceType to_promote);
    PieceColor getColor() const;
    PieceType getType() const;
    bool operator==(const Piece& that) const;
    bool operator!=(const Piece& that) const;
};
