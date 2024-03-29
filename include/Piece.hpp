#pragma once

struct Piece {
public:
    enum Color { White = 0,
        Black = 1 };
    enum Type { King = 0,
        Queen = 1,
        Bishop = 2,
        Knight = 3,
        Rook = 4,
        Pawn = 5 };
    Color color;
    Type type;
    bool moved;
    Piece(Color _color, Type _type);
};
