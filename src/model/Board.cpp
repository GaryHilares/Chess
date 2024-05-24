#include "../../include/model/Board.hpp"
#include <stdexcept>

Board::Board()
    : m_board({ Piece(PieceColor::Black, PieceType::Rook), Piece(PieceColor::Black, PieceType::Pawn), std::nullopt, std::nullopt, std::nullopt, std::nullopt, Piece(PieceColor::White, PieceType::Pawn), Piece(PieceColor::White, PieceType::Rook),
          Piece(PieceColor::Black, PieceType::Knight), Piece(PieceColor::Black, PieceType::Pawn), std::nullopt, std::nullopt, std::nullopt, std::nullopt, Piece(PieceColor::White, PieceType::Pawn), Piece(PieceColor::White, PieceType::Knight),
          Piece(PieceColor::Black, PieceType::Bishop), Piece(PieceColor::Black, PieceType::Pawn), std::nullopt, std::nullopt, std::nullopt, std::nullopt, Piece(PieceColor::White, PieceType::Pawn), Piece(PieceColor::White, PieceType::Bishop),
          Piece(PieceColor::Black, PieceType::Queen), Piece(PieceColor::Black, PieceType::Pawn), std::nullopt, std::nullopt, std::nullopt, std::nullopt, Piece(PieceColor::White, PieceType::Pawn), Piece(PieceColor::White, PieceType::Queen),
          Piece(PieceColor::Black, PieceType::King), Piece(PieceColor::Black, PieceType::Pawn), std::nullopt, std::nullopt, std::nullopt, std::nullopt, Piece(PieceColor::White, PieceType::Pawn), Piece(PieceColor::White, PieceType::King),
          Piece(PieceColor::Black, PieceType::Bishop), Piece(PieceColor::Black, PieceType::Pawn), std::nullopt, std::nullopt, std::nullopt, std::nullopt, Piece(PieceColor::White, PieceType::Pawn), Piece(PieceColor::White, PieceType::Bishop),
          Piece(PieceColor::Black, PieceType::Knight), Piece(PieceColor::Black, PieceType::Pawn), std::nullopt, std::nullopt, std::nullopt, std::nullopt, Piece(PieceColor::White, PieceType::Pawn), Piece(PieceColor::White, PieceType::Knight),
          Piece(PieceColor::Black, PieceType::Rook), Piece(PieceColor::Black, PieceType::Pawn), std::nullopt, std::nullopt, std::nullopt, std::nullopt, Piece(PieceColor::White, PieceType::Pawn), Piece(PieceColor::White, PieceType::Rook) })
{
}

std::optional<Piece>& Board::accessBoard(const BoardCoordinate pos)
{
    return accessBoard(pos.getCol(), pos.getRow());
}

std::optional<Piece>& Board::accessBoard(short int col, short int row)
{
    if (col > 8 || col < 1 || row > 8 || row < 1) {
        throw std::out_of_range("Out of board's range.");
    }
    return m_board[(col - 1) * 8 + (8 - row)];
}

std::optional<Piece> Board::readBoard(const BoardCoordinate pos) const
{
    return readBoard(pos.getCol(), pos.getRow());
}

std::optional<Piece> Board::readBoard(short int col, short int row) const
{
    if (col > 8 || col < 1 || row > 8 || row < 1) {
        return std::nullopt;
    }
    return m_board[(col - 1) * 8 + (8 - row)];
}