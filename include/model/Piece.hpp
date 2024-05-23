#pragma once

enum class PieceColor {
    White = 0,
    Black = 1
};
enum class PieceType {
    King = 0,
    Queen = 1,
    Bishop = 2,
    Knight = 3,
    Rook = 4,
    Pawn = 5
};

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
