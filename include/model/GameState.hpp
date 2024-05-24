#pragma once
#include "Board.hpp"
#include "BoardCoordinate.hpp"
#include "Piece.hpp"
#include <array>
#include <optional>

/// TODO: Disable castling when a player's king is in check.
class GameState {
private:
    PieceColor m_turn_color;
    Board m_board;
    std::optional<Piece> pawn_double_moved_last_turn;
    std::optional<Piece>& accessBoard(short int col, short int row);

    /**
     * @brief Returns true if there is interruptions between the source and the destiny.
     * @details If coordinates are in a column, row, or diagonal, checks if all of the squares between them (exclusive)
     *          are empty. If they are, returns false. If they are not empty or if the coordinates are not in a column,
     *          row or diagonal, returns true.
     * @param source The source from where start checking.
     * @param destiny The target to finish checking.
     * @return True if there are interruptions, false otherwise.
     */
    bool existInterrumptions(BoardCoordinate source, BoardCoordinate destiny) const;
    void changeTurnColor();
    bool belongsToCurrentPlayer(const Piece& piece) const;
    BoardCoordinate findKingPosition(PieceColor color) const;

public:
    GameState();
    std::optional<Piece> readBoard(const BoardCoordinate pos) const;
    std::optional<Piece> readBoard(short int col, short int row) const;
    bool isLegalMove(const BoardCoordinate source, const BoardCoordinate destiny) const;
    void move(const BoardCoordinate source, const BoardCoordinate destiny);
};
