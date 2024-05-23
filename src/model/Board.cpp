#include "../../include/model/Board.hpp"

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

std::optional<Piece>& Board::accessBoard(short int col, short int row)
{
    if (col > 7 || col < 0 || row > 7 || row < 0) {
        throw std::out_of_range("Out of board's range.");
    }
    return m_board[col * 8 + row];
}

std::optional<Piece> Board::readBoard(const BoardCoordinate pos) const
{
    return readBoard(pos.getCol(), pos.getRow());
}

std::optional<Piece> Board::readBoard(short int col, short int row) const
{
    if (col > 7 || col < 0 || row > 7 || row < 0) {
        return std::nullopt;
    }
    return m_board[col * 8 + row];
}