#pragma once
#include "BoardCoordinate.hpp"
#include "Piece.hpp"
#include <array>
#include <optional>

class Board {
private:
    std::array<std::optional<Piece>, 64> m_board;

public:
    Board();
    std::optional<Piece>& accessBoard(const BoardCoordinate pos);
    std::optional<Piece>& accessBoard(short int col, short int row);
    std::optional<Piece> readBoard(const BoardCoordinate pos) const;
    std::optional<Piece> readBoard(short int col, short int row) const;
};
