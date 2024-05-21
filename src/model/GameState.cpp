#include "../../include/model/GameState.hpp"
#include <stdexcept>

GameState::GameState()
    : board({ new Piece(PieceColor::Black, PieceType::Rook), new Piece(PieceColor::Black, PieceType::Pawn), nullptr, nullptr, nullptr, nullptr, new Piece(PieceColor::White, PieceType::Pawn), new Piece(PieceColor::White, PieceType::Rook),
          new Piece(PieceColor::Black, PieceType::Knight), new Piece(PieceColor::Black, PieceType::Pawn), nullptr, nullptr, nullptr, nullptr, new Piece(PieceColor::White, PieceType::Pawn), new Piece(PieceColor::White, PieceType::Knight),
          new Piece(PieceColor::Black, PieceType::Bishop), new Piece(PieceColor::Black, PieceType::Pawn), nullptr, nullptr, nullptr, nullptr, new Piece(PieceColor::White, PieceType::Pawn), new Piece(PieceColor::White, PieceType::Bishop),
          new Piece(PieceColor::Black, PieceType::Queen), new Piece(PieceColor::Black, PieceType::Pawn), nullptr, nullptr, nullptr, nullptr, new Piece(PieceColor::White, PieceType::Pawn), new Piece(PieceColor::White, PieceType::Queen),
          new Piece(PieceColor::Black, PieceType::King), new Piece(PieceColor::Black, PieceType::Pawn), nullptr, nullptr, nullptr, nullptr, new Piece(PieceColor::White, PieceType::Pawn), new Piece(PieceColor::White, PieceType::King),
          new Piece(PieceColor::Black, PieceType::Bishop), new Piece(PieceColor::Black, PieceType::Pawn), nullptr, nullptr, nullptr, nullptr, new Piece(PieceColor::White, PieceType::Pawn), new Piece(PieceColor::White, PieceType::Bishop),
          new Piece(PieceColor::Black, PieceType::Knight), new Piece(PieceColor::Black, PieceType::Pawn), nullptr, nullptr, nullptr, nullptr, new Piece(PieceColor::White, PieceType::Pawn), new Piece(PieceColor::White, PieceType::Knight),
          new Piece(PieceColor::Black, PieceType::Rook), new Piece(PieceColor::Black, PieceType::Pawn), nullptr, nullptr, nullptr, nullptr, new Piece(PieceColor::White, PieceType::Pawn), new Piece(PieceColor::White, PieceType::Rook) })
    , turn_color(PieceColor::White)
    , pawn_double_moved_last_turn(nullptr)
{
}

Piece*& GameState::accessBoard(short int col, short int row)
{
    if (col > 7 || col < 0 || row > 7 || row < 0) {
        throw std::out_of_range("Out of board's range.");
    }
    return board[col * 8 + row];
}

Piece* GameState::readBoard(short int col, short int row) const
{
    if (col > 7 || col < 0 || row > 7 || row < 0) {
        return nullptr;
    }
    return board[col * 8 + row];
}

bool GameState::existInterrumptions(BoardCoordinate source, BoardCoordinate destiny) const
{
    if (source.getCol() == destiny.getCol()) {
        short int modifier = source.getRow() < destiny.getRow() ? 1 : -1;
        for (short int i = source.getRow(); i != destiny.getRow(); i += modifier)
            if (readBoard(source.getCol(), i) != nullptr)
                return true;
        return false;
    } else if (source.getRow() == destiny.getRow()) {
        short int modifier = source.getCol() < destiny.getCol() ? 1 : -1;
        for (short int i = source.getCol(); i != destiny.getCol(); i += modifier)
            if (board[i * 8 + source.getRow()] != nullptr)
                return true;
        return false;
    } else if (abs(source.getRow() - destiny.getRow()) == abs(source.getCol() - destiny.getCol())) {
        short int modifierX = source.getCol() < destiny.getCol() ? 1 : -1;
        short int modifierY = source.getRow() < destiny.getRow() ? 1 : -1;
        for (short int i = source.getCol(), j = source.getRow(); i != destiny.getCol() || j != destiny.getRow(); i += modifierX, j += modifierY)
            if (board[i * 8 + j] != nullptr)
                return true;
        return false;
    } else {
        return true;
    }
}

PieceColor GameState::getTurnColor() const
{
    return turn_color;
}

void GameState::changeTurnColor()
{
    turn_color = turn_color == PieceColor::White ? PieceColor::Black : PieceColor::White;
}