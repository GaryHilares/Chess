#pragma once
#include "piece.hpp"
#include <array>

class gameController
{
private:
    std::array<piece*,64> board;
    piece::Color turnColor;
    piece* pawnDoubleMovedLastTurn;
    friend class gameUI;
public:
    gameController();
};
