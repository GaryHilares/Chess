#include "../../include/model/GameState.hpp"
#include <stdexcept>

GameState::GameState()
    : m_turn_color(PieceColor::White)
    , m_board()
    , pawn_double_moved_last_turn(std::nullopt)
{
}

std::optional<Piece>& GameState::accessBoard(short int col, short int row)
{
    return m_board.accessBoard(col, row);
}

std::optional<Piece> GameState::readBoard(BoardCoordinate pos) const
{
    return m_board.readBoard(pos);
}

std::optional<Piece> GameState::readBoard(short int col, short int row) const
{
    return m_board.readBoard(col, row);
}

bool GameState::existInterrumptions(BoardCoordinate source, BoardCoordinate destiny) const
{
    if (source.getCol() == destiny.getCol()) {
        short int modifier = source.getRow() < destiny.getRow() ? 1 : -1;
        for (short int i = source.getRow(); i != destiny.getRow(); i += modifier) {
            if (readBoard(source.getCol(), i).has_value()) {
                return true;
            }
        }
        return false;
    } else if (source.getRow() == destiny.getRow()) {
        short int modifier = source.getCol() < destiny.getCol() ? 1 : -1;
        for (short int i = source.getCol(); i != destiny.getCol(); i += modifier) {
            if (readBoard(i, source.getRow()).has_value()) {
                return true;
            }
        }
        return false;
    } else if (abs(source.getRow() - destiny.getRow()) == abs(source.getCol() - destiny.getCol())) {
        short int modifierX = source.getCol() < destiny.getCol() ? 1 : -1;
        short int modifierY = source.getRow() < destiny.getRow() ? 1 : -1;
        for (short int i = source.getCol(), j = source.getRow(); i != destiny.getCol() || j != destiny.getRow(); i += modifierX, j += modifierY) {
            if (readBoard(i, j).has_value()) {
                return true;
            }
        }
        return false;
    } else {
        return true;
    }
}

bool GameState::belongsToCurrentPlayer(const Piece& piece) const
{
    return piece.getColor() == m_turn_color;
}

void GameState::changeTurnColor()
{
    m_turn_color = m_turn_color == PieceColor::White ? PieceColor::Black : PieceColor::White;
}

BoardCoordinate GameState::findKingPosition(PieceColor color) const
{
    for (short int i = 1; i <= 8; i++) {
        for (short int j = 1; j <= 8; j++) {
            BoardCoordinate cur(static_cast<BoardCoordinate::Column>(i), j);
            std::optional<Piece> piece_at_cur = readBoard(cur);
            if (piece_at_cur.has_value()
                && piece_at_cur.value().getType() == PieceType::King
                && piece_at_cur.value().getColor() == color) {
                return cur;
            }
        }
    }
    throw std::runtime_error("No king of given color found");
}

bool GameState::isLegalMove(const BoardCoordinate source, const BoardCoordinate destiny) const
{
    const Piece moving_piece = *this->readBoard(source.getCol(), source.getRow());
    // Check that moved piece belongs to current player.
    if (!belongsToCurrentPlayer(moving_piece)) {
        return false;
    }

    // Check that destiny square does not have a piece belonging to the current player
    if (readBoard(destiny).has_value()
        && belongsToCurrentPlayer(readBoard(destiny).value())) {
        return false;
    }

    // Get delta X, delta Y, the position of the current player's king, and the opposing color.
    const short int movedX = abs(source.getCol() - destiny.getCol());
    const short int movedY = abs(source.getRow() - destiny.getRow());
    const BoardCoordinate king_position = findKingPosition(m_turn_color);
    const PieceColor opposing_color = m_turn_color == PieceColor::Black ? PieceColor::White : PieceColor::Black;

    // Check that opposing knights will not keep making check after movement.
    for (std::pair<short int, short int> knightPlaces : std::initializer_list<std::pair<short int, short int>> { { -2, -1 }, { -1, -2 }, { -1, 2 }, { 2, -1 }, { 1, 2 }, { 2, 1 }, { 1, -2 }, { -2, 1 } }) {
        const std::optional<Piece> piece_at_offset = readBoard(king_position.getCol() + knightPlaces.first, king_position.getRow() + knightPlaces.second);
        if (!(destiny.getCol() == king_position.getCol() + knightPlaces.first
                && destiny.getRow() == king_position.getRow() + knightPlaces.second)
            && piece_at_offset.has_value()
            && piece_at_offset.value().getType() == PieceType::Knight
            && piece_at_offset.value().getColor() == opposing_color) {
            return false;
        }
    }

    // Check that opposing rooks, bishops and queens will not keep making check after movement.
    for (std::pair<short int, short int> modifiers : std::initializer_list<std::pair<short int, short int>> { { -1, -1 }, { 1, -1 }, { -1, 1 }, { 1, 1 }, { 0, 1 }, { 0, -1 }, { 1, 0 }, { -1, 0 } }) {
        for (int x = king_position.getCol() + modifiers.first, y = king_position.getRow() + modifiers.second; x < 8 && x >= 0 && y < 8 && y >= 0; x += modifiers.first, y += modifiers.second) {
            std::optional<Piece> piece_at_offset = readBoard(x, y);
            if (piece_at_offset.has_value()) {
                if (piece_at_offset.value().getColor() == opposing_color
                    && !(destiny.getCol() == x && destiny.getRow() == y)
                    && (readBoard(x, y)->getType() == PieceType::Queen
                        || ((abs(modifiers.first) != abs(modifiers.second)
                                && piece_at_offset.value().getType() == PieceType::Rook)
                            || (abs(modifiers.first) == abs(modifiers.second)
                                && piece_at_offset.value().getType() == PieceType::Bishop)))) {
                    return false;
                } else {
                    break;
                }
            } else if (destiny.getCol() == x && destiny.getRow() == y) {
                break;
            }
        }
    }

    // Check that opposing pawns will not keep making check after movement.
    for (int i : { -1, 1 }) {
        if (readBoard(king_position.getCol() + i, king_position.getRow() + (opposing_color == PieceColor::White ? 1 : -1)).has_value()
            && readBoard(king_position.getCol() + i, king_position.getRow() + (opposing_color == PieceColor::White ? 1 : -1))->getColor() == opposing_color
            && readBoard(king_position.getCol() + i, king_position.getRow() + (opposing_color == PieceColor::White ? 1 : -1))->getType() == PieceType::Pawn) {
            return false;
        }
    }

    // Check that piece can reach destination.
    switch (moving_piece.getType()) {
    case PieceType::King:
        if (existInterrumptions(source, destiny)) {
            return false;
        }
        if (destiny.getCol() == BoardCoordinate::columnToInt(BoardCoordinate::Column::G)
            && readBoard(BoardCoordinate::columnToInt(BoardCoordinate::Column::H), source.getRow()).has_value() && !readBoard(BoardCoordinate::columnToInt(BoardCoordinate::Column::H), source.getRow())->hasMoved()
            && movedY == 0
            && !moving_piece.hasMoved()
            && isLegalMove(source, BoardCoordinate(BoardCoordinate::Column::F, moving_piece.getColor() == PieceColor::White ? 1 : 8))) {
            return true;
        }
        if (destiny.getCol() == BoardCoordinate::columnToInt(BoardCoordinate::Column::C)
            && readBoard(BoardCoordinate::columnToInt(BoardCoordinate::Column::A), source.getRow()).has_value()
            && !readBoard(BoardCoordinate::columnToInt(BoardCoordinate::Column::A), source.getRow()).value().hasMoved()
            && movedY == 0
            && !moving_piece.hasMoved()
            && isLegalMove(source, BoardCoordinate(BoardCoordinate::Column::D, moving_piece.getColor() == PieceColor::White ? 1 : 8))) {
            return true;
        }
        if (movedX > 1 || movedY > 1) {
            return false;
        }
        return true;
    case PieceType::Queen:
        if (existInterrumptions(source, destiny)) {
            return false;
        }
        return true;
    case PieceType::Bishop:
        if (movedX != movedY) {
            return false;
        }
        if (existInterrumptions(source, destiny)) {
            return false;
        }
        return true;
    case PieceType::Knight:
        if (!((movedX == 2 && movedY == 1) || (movedX == 1 && movedY == 2))) {
            return false;
        }
        return true;
    case PieceType::Rook:
        if (movedX != 0 && movedY != 0) {
            return false;
        }
        if (existInterrumptions(source, destiny)) {
            return false;
        }
        return true;
    case PieceType::Pawn:
        if ((moving_piece.getColor() == PieceColor::White && BoardCoordinate::rowToInt(source.getRow()) > BoardCoordinate::rowToInt(destiny.getRow())) || (moving_piece.getColor() == PieceColor::Black && BoardCoordinate::rowToInt(source.getRow()) < BoardCoordinate::rowToInt(destiny.getRow()))) {
            return false;
        }
        if (existInterrumptions(source, destiny)) {
            return false;
        }
        if (((moving_piece.getColor() == PieceColor::White && source.getRow() == BoardCoordinate::rowToInt(2)) || (moving_piece.getColor() == PieceColor::Black && source.getRow() == BoardCoordinate::rowToInt(7)))
            && movedX == 0 && movedY == 2 && !readBoard(destiny.getCol(), destiny.getRow()).has_value()) {
            return true;
        } else if (movedX == 1
            && movedY == 1
            && (readBoard(destiny.getCol(), destiny.getRow()).has_value()
                || (source.getRow() == BoardCoordinate::rowToInt(5)
                    && moving_piece.getColor() == PieceColor::White
                    && readBoard(destiny.getCol(), destiny.getRow() + 1).has_value()
                    && readBoard(destiny.getCol(), destiny.getRow() + 1)->getType() == PieceType::Pawn
                    && readBoard(destiny.getCol(), destiny.getRow() + 1) == this->pawn_double_moved_last_turn)
                || (source.getRow() == BoardCoordinate::rowToInt(4)
                    && moving_piece.getColor() == PieceColor::Black
                    && readBoard(destiny.getCol(), destiny.getRow() - 1).has_value()
                    && readBoard(destiny.getCol(), destiny.getRow() - 1)->getType() == PieceType::Pawn
                    && readBoard(destiny.getCol(), destiny.getRow() - 1) == this->pawn_double_moved_last_turn))) {
            return true;
        } else if (movedX != 0 || movedY != 1)
            return false;
        return true;
    default:
        return false;
    }
}

void GameState::move(const BoardCoordinate source, const BoardCoordinate destiny)
{
    std::optional<Piece>& moving_piece = accessBoard(source.getCol(), destiny.getRow());

    // Move piece.
    pawn_double_moved_last_turn.reset();
    moving_piece->setAsMoved();
    if (moving_piece->getType() == PieceType::Pawn
        && source.getCol() != destiny.getCol()
        && !readBoard(destiny.getCol(), destiny.getRow()).has_value()) {
        // Check if move is en passant, and if it is remove the pawn.
        if (moving_piece->getColor() == PieceColor::White
            && readBoard(destiny.getCol(), destiny.getRow() + 1).has_value()) {
            accessBoard(destiny.getCol(), destiny.getRow() + 1).reset();
        } else if (moving_piece->getColor() == PieceColor::Black
            && readBoard(destiny.getCol(), destiny.getRow() - 1).has_value()) {
            accessBoard(destiny.getCol(), destiny.getRow() - 1).reset();
        }
    }
    if (readBoard(destiny.getCol(), destiny.getRow()).has_value())
        // Check if move is a capture, and if it is, remove the targetted piece.
        accessBoard(destiny.getCol(), destiny.getRow()).reset();
    if (moving_piece->getType() == PieceType::King
        && source.getCol() == BoardCoordinate::columnToInt(BoardCoordinate::Column::E)
        && destiny.getCol() == BoardCoordinate::columnToInt(BoardCoordinate::Column::C)) {
        // Check if move is long castling, and if it is, move the rook.
        accessBoard(BoardCoordinate::columnToInt(BoardCoordinate::Column::A), source.getRow())->setAsMoved();
        std::swap(accessBoard(BoardCoordinate::columnToInt(BoardCoordinate::Column::A), source.getRow()),
            accessBoard(BoardCoordinate::columnToInt(BoardCoordinate::Column::D), source.getRow()));
    } else if (moving_piece->getType() == PieceType::King
        && source.getCol() == BoardCoordinate::columnToInt(BoardCoordinate::Column::E)
        && destiny.getCol() == BoardCoordinate::columnToInt(BoardCoordinate::Column::G)) {
        // Check if move is short castling, and if it is, move the rook.
        this->accessBoard(BoardCoordinate::columnToInt(BoardCoordinate::Column::H), source.getRow())->setAsMoved();
        std::swap(accessBoard(BoardCoordinate::columnToInt(BoardCoordinate::Column::H), source.getRow()), accessBoard(BoardCoordinate::columnToInt(BoardCoordinate::Column::F), source.getRow()));
    } else if (moving_piece->getType() == PieceType::Pawn
        && destiny.getRow() == BoardCoordinate::rowToInt(moving_piece->getColor() == PieceColor::White ? 8 : 1)) {
        // Check if move is promotion, and if it is, display promotion menu.
        /*PieceType toPromote;
        while (true) {
            sf::Event event;
            if (window.pollEvent(event)) {
                switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    exit(0);
                default:
                    break;
                }
            }
            window.clear(moving_piece->getColor() == PieceColor::White ? sf::Color::White : sf::Color(100, 100, 100));
            sf::Transformable transformer;
            sf::RenderStates specificState;
            transformer.setPosition(100, 350);
            for (PieceType i : { PieceType::Queen, PieceType::Rook, PieceType::Bishop, PieceType::Knight }) {
                transformer.move(100, 0);
                specificState.transform = transformer.getTransform();
                window.draw(this->pieces_sprites[static_cast<int>(moving_piece->getColor()) * 6 + static_cast<int>(i)), specificState);
            }
            window.display();
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                sf::Vector2f MousePosF = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                sf::Vector2i MousePos(MousePosF.x, MousePosF.y);
                if (MousePos.y >= 350 && MousePos.y <= 450 && MousePos.x >= 200 && MousePos.x <= 600) {
                    switch ((MousePos.x - 200) / 100) {
                    case 0:
                        toPromote = PieceType::Queen;
                        break;
                    case 1:
                        toPromote = PieceType::Rook;
                        break;
                    case 2:
                        toPromote = PieceType::Bishop;
                        break;
                    case 3:
                        toPromote = PieceType::Knight;
                        break;
                    case 4:
                        throw std::out_of_range("Out of promotable Pieces");
                    }
                    break;
                }
            }
        }
        moving_piece->promotePawnTo(toPromote);*/
    } else if (moving_piece->getType() == PieceType::Pawn
        && abs(source.getRow() - destiny.getRow()) == 2) {
        // If pawn double moved, store moved pawn.
        this->pawn_double_moved_last_turn = moving_piece;
    }
    // Move piece
    accessBoard(destiny.getCol(), destiny.getRow()) = moving_piece;

    // Change turn color
    changeTurnColor();
}