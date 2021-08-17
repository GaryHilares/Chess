#include "../include/gameController.hpp"

gameController::gameController(): board({
new piece(piece::Black,piece::Rook),new piece(piece::Black,piece::Pawn),nullptr,nullptr,nullptr,nullptr,new piece(piece::White,piece::Pawn),new piece(piece::White,piece::Rook),
new piece(piece::Black,piece::Knight),new piece(piece::Black,piece::Pawn),nullptr,nullptr,nullptr,nullptr,new piece(piece::White,piece::Pawn),new piece(piece::White,piece::Knight),
new piece(piece::Black,piece::Bishop),new piece(piece::Black,piece::Pawn),nullptr,nullptr,nullptr,nullptr,new piece(piece::White,piece::Pawn),new piece(piece::White,piece::Bishop),
new piece(piece::Black,piece::Queen),new piece(piece::Black,piece::Pawn),nullptr,nullptr,nullptr,nullptr,new piece(piece::White,piece::Pawn),new piece(piece::White,piece::Queen),
new piece(piece::Black,piece::King),new piece(piece::Black,piece::Pawn),nullptr,nullptr,nullptr,nullptr,new piece(piece::White,piece::Pawn),new piece(piece::White,piece::King),
new piece(piece::Black,piece::Bishop),new piece(piece::Black,piece::Pawn),nullptr,nullptr,nullptr,nullptr,new piece(piece::White,piece::Pawn),new piece(piece::White,piece::Bishop),
new piece(piece::Black,piece::Knight),new piece(piece::Black,piece::Pawn),nullptr,nullptr,nullptr,nullptr,new piece(piece::White,piece::Pawn),new piece(piece::White,piece::Knight),
new piece(piece::Black,piece::Rook),new piece(piece::Black,piece::Pawn),nullptr,nullptr,nullptr,nullptr,new piece(piece::White,piece::Pawn),new piece(piece::White,piece::Rook)
}), turnColor(piece::Color::White), pawnDoubleMovedLastTurn(nullptr){}
