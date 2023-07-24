#include "../include/GameController.hpp"

GameController::GameController()
    : board({ new Piece(Piece::Black, Piece::Rook), new Piece(Piece::Black, Piece::Pawn), nullptr, nullptr, nullptr, nullptr, new Piece(Piece::White, Piece::Pawn), new Piece(Piece::White, Piece::Rook),
        new Piece(Piece::Black, Piece::Knight), new Piece(Piece::Black, Piece::Pawn), nullptr, nullptr, nullptr, nullptr, new Piece(Piece::White, Piece::Pawn), new Piece(Piece::White, Piece::Knight),
        new Piece(Piece::Black, Piece::Bishop), new Piece(Piece::Black, Piece::Pawn), nullptr, nullptr, nullptr, nullptr, new Piece(Piece::White, Piece::Pawn), new Piece(Piece::White, Piece::Bishop),
        new Piece(Piece::Black, Piece::Queen), new Piece(Piece::Black, Piece::Pawn), nullptr, nullptr, nullptr, nullptr, new Piece(Piece::White, Piece::Pawn), new Piece(Piece::White, Piece::Queen),
        new Piece(Piece::Black, Piece::King), new Piece(Piece::Black, Piece::Pawn), nullptr, nullptr, nullptr, nullptr, new Piece(Piece::White, Piece::Pawn), new Piece(Piece::White, Piece::King),
        new Piece(Piece::Black, Piece::Bishop), new Piece(Piece::Black, Piece::Pawn), nullptr, nullptr, nullptr, nullptr, new Piece(Piece::White, Piece::Pawn), new Piece(Piece::White, Piece::Bishop),
        new Piece(Piece::Black, Piece::Knight), new Piece(Piece::Black, Piece::Pawn), nullptr, nullptr, nullptr, nullptr, new Piece(Piece::White, Piece::Pawn), new Piece(Piece::White, Piece::Knight),
        new Piece(Piece::Black, Piece::Rook), new Piece(Piece::Black, Piece::Pawn), nullptr, nullptr, nullptr, nullptr, new Piece(Piece::White, Piece::Pawn), new Piece(Piece::White, Piece::Rook) })
    , turn_color(Piece::Color::White)
    , pawn_double_moved_last_turn(nullptr)
{
}
