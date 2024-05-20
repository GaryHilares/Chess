#include "../include/GameController.hpp"

ChessGame::ChessGame()
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
