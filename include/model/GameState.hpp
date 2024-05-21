#pragma once
#include "../Square.hpp"
#include "Piece.hpp"
#include <array>
#include <optional>

/// TODO: Disable castling when a player's king is in check.
class GameState {
    PieceColor turn_color;

public:
    std::array<Piece*, 64> board;
    Piece* pawn_double_moved_last_turn;
    GameState();
    bool existInterrumptions(Square source, Square destiny) const;
    Piece*& accessBoard(short int col, short int row);
    Piece* readBoard(short int col, short int row) const;
    PieceColor getTurnColor() const;
    void changeTurnColor();
};
