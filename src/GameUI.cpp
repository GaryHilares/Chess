#include "../include/GameUI.hpp"

void GameUI::update(sf::RenderWindow& window)
{
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    if ((sf::Mouse::isButtonPressed(sf::Mouse::Left) && !this->last_square.has_value())) {
        // If not grabbing piece, pick up piece if it has started moving
        this->last_square = getSquare(mousePos.x, mousePos.y);
    } else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->last_square.has_value()) {
        // If piece is dropped, try to move it to current square.
        BoardCoordinate pressedSquare = getSquare(mousePos.x, mousePos.y);
        Piece*& moving_piece = this->m_game.accessBoard(this->last_square.value().getCol(), this->last_square.value().getRow());
        if (m_game.isLegalMove(*moving_piece, this->last_square.value(), pressedSquare)) {
            // Move piece.
            this->m_game.pawn_double_moved_last_turn = nullptr;
            moving_piece->setAsMoved();
            if (moving_piece->getType() == PieceType::Pawn
                && this->last_square.value().getCol() != pressedSquare.getCol()
                && this->m_game.readBoard(pressedSquare.getCol(), pressedSquare.getRow()) == nullptr) {
                // Check if move is en passant, and if it is remove the pawn.
                if (moving_piece->getColor() == PieceColor::White && this->m_game.board[pressedSquare.getCol() * 8 + pressedSquare.getRow() + 1] != nullptr) {
                    delete this->m_game.board[pressedSquare.getCol() * 8 + pressedSquare.getRow() + 1];
                    this->m_game.board[pressedSquare.getCol() * 8 + pressedSquare.getRow() + 1] = nullptr;
                } else if (moving_piece->getColor() == PieceColor::Black && this->m_game.board[pressedSquare.getCol() * 8 + pressedSquare.getRow() - 1] != nullptr) {
                    delete this->m_game.board[pressedSquare.getCol() * 8 + pressedSquare.getRow() - 1];
                    this->m_game.board[pressedSquare.getCol() * 8 + pressedSquare.getRow() - 1] = nullptr;
                }
            }
            if (this->m_game.board[pressedSquare.getCol() * 8 + pressedSquare.getRow()] != nullptr)
                // Check if move is a capture, and if it is, remove the targetted piece.
                delete this->m_game.board[pressedSquare.getCol() * 8 + pressedSquare.getRow()];
            if (moving_piece->getType() == PieceType::King
                && this->last_square.value().getCol() == BoardCoordinate::columnToInt(BoardCoordinate::Column::E)
                && pressedSquare.getCol() == BoardCoordinate::columnToInt(BoardCoordinate::Column::C)) {
                // Check if move is long castling, and if it is, move the rook.
                this->m_game.board[BoardCoordinate::columnToInt(BoardCoordinate::Column::A) * 8 + this->last_square.value().getRow()]->setAsMoved();
                std::swap(this->m_game.board[BoardCoordinate::columnToInt(BoardCoordinate::Column::A) * 8 + this->last_square.value().getRow()], this->m_game.board[BoardCoordinate::columnToInt(BoardCoordinate::Column::D) * 8 + this->last_square.value().getRow()]);
            } else if (moving_piece->getType() == PieceType::King
                && this->last_square.value().getCol() == BoardCoordinate::columnToInt(BoardCoordinate::Column::E)
                && pressedSquare.getCol() == BoardCoordinate::columnToInt(BoardCoordinate::Column::G)) {
                // Check if move is short castling, and if it is, move the rook.
                this->m_game.board[BoardCoordinate::columnToInt(BoardCoordinate::Column::H) * 8 + this->last_square.value().getRow()]->setAsMoved();
                std::swap(this->m_game.board[BoardCoordinate::columnToInt(BoardCoordinate::Column::H) * 8 + this->last_square.value().getRow()], this->m_game.board[BoardCoordinate::columnToInt(BoardCoordinate::Column::F) * 8 + this->last_square.value().getRow()]);
            } else if (moving_piece->getType() == PieceType::Pawn
                && pressedSquare.getRow() == BoardCoordinate::rowToInt(moving_piece->getColor() == PieceColor::White ? 8 : 1)) {
                // Check if move is promotion, and if it is, display promotion menu.
                PieceType toPromote;
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
                        window.draw(this->pieces_sprites[static_cast<int>(moving_piece->getColor()) * 6 + static_cast<int>(i)], specificState);
                    }
                    window.display();
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                        sf::Vector2f newMousePosF = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                        sf::Vector2i newMousePos(newMousePosF.x, newMousePosF.y);
                        if (newMousePos.y >= 350 && newMousePos.y <= 450 && newMousePos.x >= 200 && newMousePos.x <= 600) {
                            switch ((newMousePos.x - 200) / 100) {
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
                moving_piece->promotePawnTo(toPromote);
            } else if (moving_piece->getType() == PieceType::Pawn
                && abs(this->last_square.value().getRow() - pressedSquare.getRow()) == 2) {
                // If pawn double moved, store moved pawn.
                this->m_game.pawn_double_moved_last_turn = moving_piece;
            }
            // Move piece
            this->m_game.board[pressedSquare.getCol() * 8 + pressedSquare.getRow()] = moving_piece;

            // Remove moving state
            moving_piece = nullptr;
            this->last_square.reset();

            // Change turn color
            m_game.changeTurnColor();
        } else {
            // If move is illegal, do not perform it.
            this->last_square.reset();
        }
    }
}

void GameUI::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
    Piece* moving_piece = nullptr;
    if (last_square.has_value()) {
        moving_piece = this->m_game.readBoard(this->last_square.value().getCol(), this->last_square.value().getRow());
    }
    {
        bool rectangle_is_white = false;
        sf::RectangleShape shape(sf::Vector2f(square_size, square_size));
        for (unsigned int i = board_pos_x; i < board_pos_x + square_size * 8; i += square_size) {
            for (unsigned int j = board_pos_y; j < board_pos_y + square_size * 8; j += square_size) {
                shape.setPosition(i, j);
                if (rectangle_is_white) {
                    shape.setFillColor(sf::Color(100, 100, 100, 255));
                    rectangle_is_white = false;
                } else {
                    shape.setFillColor(sf::Color::White);
                    rectangle_is_white = true;
                }
                target.draw(shape);
            }
            rectangle_is_white = !rectangle_is_white;
        }
    }
    {
        for (unsigned int i = 0; i < 8; i++) {
            for (unsigned int j = 0; j < 8; j++) {
                if (this->m_game.board[i * 8 + j] != nullptr && this->m_game.board[i * 8 + j] != moving_piece) {
                    const sf::Sprite& currentPieceSprite = this->pieces_sprites[(int)this->m_game.board[i * 8 + j]->getColor() * 6 + (int)this->m_game.board[i * 8 + j]->getType()];
                    sf::Transformable transformer;
                    sf::RenderStates specific_state = state;
                    transformer.setPosition(board_pos_x + i * square_size, board_pos_y + j * square_size);
                    specific_state.transform *= transformer.getTransform();
                    target.draw(currentPieceSprite, specific_state);
                }
            }
        }
        if (moving_piece != nullptr) {
            sf::Window* window = dynamic_cast<sf::Window*>(&target);
            if (window != nullptr) {
                const sf::Sprite& currentPieceSprite = this->pieces_sprites[(int)moving_piece->getColor() * 6 + (int)moving_piece->getType()];
                sf::Transformable transformer;
                sf::RenderStates specific_state;
                sf::Vector2f mousePos = target.mapPixelToCoords(sf::Mouse::getPosition(*window));
                sf::FloatRect spriteRectangle = currentPieceSprite.getGlobalBounds();
                transformer.setPosition(mousePos.x - board_pos_x - spriteRectangle.width / 2, mousePos.y - board_pos_y - spriteRectangle.height / 2);
                specific_state.transform = transformer.getTransform();
                target.draw(currentPieceSprite, specific_state);
            }
        }
    }
}

BoardCoordinate GameUI::getSquare(float pos_x, float pos_y)
{
    pos_x = (pos_x - board_pos_x) / square_size;
    pos_y = 8 - ((pos_y - board_pos_y) / square_size);
    int new_pos_y = pos_y + 1;
    switch ((int)pos_x) {
    case 0:
        return BoardCoordinate(BoardCoordinate::Column::A, new_pos_y);
    case 1:
        return BoardCoordinate(BoardCoordinate::Column::B, new_pos_y);
    case 2:
        return BoardCoordinate(BoardCoordinate::Column::C, new_pos_y);
    case 3:
        return BoardCoordinate(BoardCoordinate::Column::D, new_pos_y);
    case 4:
        return BoardCoordinate(BoardCoordinate::Column::E, new_pos_y);
    case 5:
        return BoardCoordinate(BoardCoordinate::Column::F, new_pos_y);
    case 6:
        return BoardCoordinate(BoardCoordinate::Column::G, new_pos_y);
    case 7:
        return BoardCoordinate(BoardCoordinate::Column::H, new_pos_y);
    default:
        throw std::out_of_range("Square is out of possible range");
    }
}

GameUI::GameUI(GameState& new_game)
    : m_game(new_game)
    , last_square(std::nullopt)
{
    if (!this->pieces_texture.loadFromFile("ChessPieces.png"))
        exit(1);
    for (unsigned int i = 0; i < 2; i++) {
        for (unsigned int j = 0; j < 6; j++) {
            this->pieces_sprites[i * 6 + j].setTexture(pieces_texture);
            this->pieces_sprites[i * 6 + j].setTextureRect(sf::IntRect(j * piece_size_in_texture, i * piece_size_in_texture, piece_size_in_texture, piece_size_in_texture));
            this->pieces_sprites[i * 6 + j].setScale(float(square_size) / piece_size_in_texture, float(square_size) / piece_size_in_texture);
        }
    }
}
