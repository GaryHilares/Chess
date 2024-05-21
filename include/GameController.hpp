#pragma once
#include "model/Piece.hpp"
#include <array>
#include <optional>

/// TODO: Disable castling when a player's king is in check.
class ChessGame {
private:
    std::array<Piece*, 64> board;
    PieceColor turn_color;
    Piece* pawn_double_moved_last_turn;
    friend class GameUI;

public:
    ChessGame();
};
