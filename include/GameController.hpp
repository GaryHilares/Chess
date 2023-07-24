#pragma once
#include "Piece.hpp"
#include <array>

class GameController {
private:
    std::array<Piece*, 64> board;
    Piece::Color turn_color;
    Piece* pawn_double_moved_last_turn;
    friend class GameUI;

public:
    GameController();
};
