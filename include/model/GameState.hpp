#pragma once
#include "BoardCoordinate.hpp"
#include "Piece.hpp"
#include <array>
#include <optional>

/// TODO: Disable castling when a player's king is in check.
class GameState {
private:
    PieceColor m_turn_color;
    std::array<Piece*, 64> m_board;
    Piece* pawn_double_moved_last_turn;
    Piece*& accessBoard(short int col, short int row);
    bool existInterrumptions(BoardCoordinate source, BoardCoordinate destiny) const;
    void changeTurnColor();

public:
    GameState();
    Piece* readBoard(short int col, short int row) const;
    PieceColor getTurnColor() const;
    bool isLegalMove(const BoardCoordinate source, const BoardCoordinate destiny) const;
    void move(const BoardCoordinate source, const BoardCoordinate destiny);
};
