#include "../include/GameUI.hpp"

bool GameUI::isLegalMove(const Piece moving, const BoardCoordinate source, const BoardCoordinate destiny) const
{
    if (moving.getColor() != m_game.getTurnColor()) {
        return false;
    }
    if (source == destiny || (m_game.readBoard(destiny.getCol(), destiny.getRow()) != nullptr && moving.getColor() == m_game.readBoard(destiny.getCol(), destiny.getRow())->getColor()))
        return false;
    const short int movedX = abs(source.getCol() - destiny.getCol());
    const short int movedY = abs(source.getRow() - destiny.getRow());
    const BoardCoordinate kingPosition = [&]() {
        for (short int i = 1; i <= 8; i++) {
            for (short int j = 1; j <= 8; j++) {
                BoardCoordinate cur(static_cast<BoardCoordinate::Column>(i), j);
                if (m_game.readBoard(cur.getCol(), cur.getRow()) != nullptr && m_game.readBoard(cur.getCol(), cur.getRow())->getType() == PieceType::King && m_game.readBoard(cur.getCol(), cur.getRow())->getColor() == moving.getColor())
                    return cur;
            }
        }
        return destiny;
    }();
    PieceColor oposingColor = moving.getColor() == PieceColor::Black ? PieceColor::White : PieceColor::Black;
    for (std::pair<short int, short int> knightPlaces : std::initializer_list<std::pair<short int, short int>> { { -2, -1 }, { -1, -2 }, { -1, 2 }, { 2, -1 }, { 1, 2 }, { 2, 1 }, { 1, -2 }, { -2, 1 } }) {
        if (m_game.readBoard(kingPosition.getCol() + knightPlaces.first, kingPosition.getRow() + knightPlaces.second) != nullptr && m_game.readBoard(kingPosition.getCol() + knightPlaces.first, kingPosition.getRow() + knightPlaces.second)->getType() == PieceType::Knight && m_game.readBoard((kingPosition.getCol() + knightPlaces.first), kingPosition.getRow() + knightPlaces.second)->getColor() == oposingColor && !(destiny.getCol() == kingPosition.getCol() + knightPlaces.first && destiny.getRow() == kingPosition.getRow() + knightPlaces.second)) {
            return false;
        }
    }
    for (std::pair<short int, short int> modifiers : std::initializer_list<std::pair<short int, short int>> { { -1, -1 }, { 1, -1 }, { -1, 1 }, { 1, 1 }, { 0, 1 }, { 0, -1 }, { 1, 0 }, { -1, 0 } }) {
        for (int x = kingPosition.getCol() + modifiers.first, y = kingPosition.getRow() + modifiers.second; x < 8 && x >= 0 && y < 8 && y >= 0; x += modifiers.first, y += modifiers.second) {
            if (m_game.readBoard(x, y) != nullptr) {
                if (m_game.readBoard(x, y)->getColor() == oposingColor
                    && !(destiny.getCol() == x && destiny.getRow() == y)
                    && (m_game.readBoard(x, y)->getType() == PieceType::Queen || ((abs(modifiers.first) != abs(modifiers.second) && m_game.readBoard(x, y)->getType() == PieceType::Rook) || (abs(modifiers.first) == abs(modifiers.second) && m_game.readBoard(x, y)->getType() == PieceType::Bishop)))) {
                    return false;
                } else
                    break;
            } else if (destiny.getCol() == x && destiny.getRow() == y) {
                break;
            }
        }
    }
    for (int i : { -1, 1 })
        if (m_game.readBoard(kingPosition.getCol() + i, kingPosition.getRow() + (oposingColor == PieceColor::White ? 1 : -1)) != nullptr && m_game.readBoard(kingPosition.getCol() + i, kingPosition.getRow() + (oposingColor == PieceColor::White ? 1 : -1))->getColor() == oposingColor && m_game.readBoard(kingPosition.getCol() + i, kingPosition.getRow() + (oposingColor == PieceColor::White ? 1 : -1))->getType() == PieceType::Pawn)
            return false;

    switch (moving.getType()) {
    case PieceType::King:
        if (m_game.existInterrumptions(source, destiny))
            return false;
        if (destiny.getCol() == BoardCoordinate::columnToInt(BoardCoordinate::Column::G) && m_game.board[BoardCoordinate::columnToInt(BoardCoordinate::Column::H) * 8 + source.getRow()] != nullptr && !m_game.board[BoardCoordinate::columnToInt(BoardCoordinate::Column::H) * 8 + source.getRow()]->hasMoved() && movedY == 0 && !moving.hasMoved() && isLegalMove(moving, source, BoardCoordinate(BoardCoordinate::Column::F, moving.getColor() == PieceColor::White ? 1 : 8)))
            return true;
        if (destiny.getCol() == BoardCoordinate::columnToInt(BoardCoordinate::Column::C) && m_game.board[BoardCoordinate::columnToInt(BoardCoordinate::Column::A) * 8 + source.getRow()] != nullptr && !m_game.board[BoardCoordinate::columnToInt(BoardCoordinate::Column::A) * 8 + source.getRow()]->hasMoved() && movedY == 0 && !moving.hasMoved() && isLegalMove(moving, source, BoardCoordinate(BoardCoordinate::Column::D, moving.getColor() == PieceColor::White ? 1 : 8)))
            return true;
        if (movedX > 1 || movedY > 1)
            return false;
        return true;
    case PieceType::Queen:
        if (m_game.existInterrumptions(source, destiny))
            return false;
        return true;
    case PieceType::Bishop:
        if (movedX != movedY)
            return false;
        if (m_game.existInterrumptions(source, destiny))
            return false;
        return true;
    case PieceType::Knight:
        if (!((movedX == 2 && movedY == 1) || (movedX == 1 && movedY == 2)))
            return false;
        return true;
    case PieceType::Rook:
        if (movedX != 0 && movedY != 0)
            return false;
        if (m_game.existInterrumptions(source, destiny))
            return false;
        return true;
    case PieceType::Pawn:
        if ((moving.getColor() == PieceColor::White && BoardCoordinate::rowToInt(source.getRow()) > BoardCoordinate::rowToInt(destiny.getRow())) || (moving.getColor() == PieceColor::Black && BoardCoordinate::rowToInt(source.getRow()) < BoardCoordinate::rowToInt(destiny.getRow())))
            return false;
        if (m_game.existInterrumptions(source, destiny))
            return false;
        if (((moving.getColor() == PieceColor::White && source.getRow() == BoardCoordinate::rowToInt(2)) || (moving.getColor() == PieceColor::Black && source.getRow() == BoardCoordinate::rowToInt(7)))
            && movedX == 0 && movedY == 2 && this->m_game.board[destiny.getCol() * 8 + destiny.getRow()] == nullptr)
            return true;
        else if (movedX == 1 && movedY == 1 && (this->m_game.board[destiny.getCol() * 8 + destiny.getRow()] != nullptr || (source.getRow() == BoardCoordinate::rowToInt(5) && moving.getColor() == PieceColor::White && this->m_game.board[destiny.getCol() * 8 + destiny.getRow() + 1] != nullptr && this->m_game.board[destiny.getCol() * 8 + destiny.getRow() + 1]->getType() == PieceType::Pawn && this->m_game.board[destiny.getCol() * 8 + destiny.getRow() + 1] == this->m_game.pawn_double_moved_last_turn) || (source.getRow() == BoardCoordinate::rowToInt(4) && moving.getColor() == PieceColor::Black && this->m_game.board[destiny.getCol() * 8 + destiny.getRow() - 1] != nullptr && this->m_game.board[destiny.getCol() * 8 + destiny.getRow() - 1]->getType() == PieceType::Pawn && this->m_game.board[destiny.getCol() * 8 + destiny.getRow() - 1] == this->m_game.pawn_double_moved_last_turn)))
            return true;
        else if (movedX != 0 || movedY != 1)
            return false;
        return true;
    default:
        return false;
    }
}

void GameUI::update(sf::RenderWindow& window)
{
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    if ((sf::Mouse::isButtonPressed(sf::Mouse::Left) && moving_piece == nullptr)) {
        this->last_square = new BoardCoordinate(getSquare(mousePos.x, mousePos.y));
        std::swap(this->moving_piece, this->m_game.board[this->last_square->getCol() * 8 + this->last_square->getRow()]);
    } else if ((!sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->moving_piece != nullptr)) {
        BoardCoordinate pressedSquare = getSquare(mousePos.x, mousePos.y);
        if (this->isLegalMove(*this->moving_piece, *this->last_square, pressedSquare)) {
            this->m_game.pawn_double_moved_last_turn = nullptr;
            this->moving_piece->setAsMoved();
            if (this->moving_piece->getType() == PieceType::Pawn && this->last_square->getCol() != pressedSquare.getCol() && this->m_game.board[pressedSquare.getCol() * 8 + pressedSquare.getRow()] == nullptr) {
                if (this->moving_piece->getColor() == PieceColor::White && this->m_game.board[pressedSquare.getCol() * 8 + pressedSquare.getRow() + 1] != nullptr) {
                    delete this->m_game.board[pressedSquare.getCol() * 8 + pressedSquare.getRow() + 1];
                    this->m_game.board[pressedSquare.getCol() * 8 + pressedSquare.getRow() + 1] = nullptr;
                } else if (this->moving_piece->getColor() == PieceColor::Black && this->m_game.board[pressedSquare.getCol() * 8 + pressedSquare.getRow() - 1] != nullptr) {
                    delete this->m_game.board[pressedSquare.getCol() * 8 + pressedSquare.getRow() - 1];
                    this->m_game.board[pressedSquare.getCol() * 8 + pressedSquare.getRow() - 1] = nullptr;
                }
            }
            if (this->m_game.board[pressedSquare.getCol() * 8 + pressedSquare.getRow()] != nullptr)
                delete this->m_game.board[pressedSquare.getCol() * 8 + pressedSquare.getRow()];
            if (this->moving_piece->getType() == PieceType::King && this->last_square->getCol() == BoardCoordinate::columnToInt(BoardCoordinate::Column::E) && pressedSquare.getCol() == BoardCoordinate::columnToInt(BoardCoordinate::Column::C)) {
                this->m_game.board[BoardCoordinate::columnToInt(BoardCoordinate::Column::A) * 8 + this->last_square->getRow()]->setAsMoved();
                std::swap(this->m_game.board[BoardCoordinate::columnToInt(BoardCoordinate::Column::A) * 8 + this->last_square->getRow()], this->m_game.board[BoardCoordinate::columnToInt(BoardCoordinate::Column::D) * 8 + this->last_square->getRow()]);

            } else if (this->moving_piece->getType() == PieceType::King && this->last_square->getCol() == BoardCoordinate::columnToInt(BoardCoordinate::Column::E) && pressedSquare.getCol() == BoardCoordinate::columnToInt(BoardCoordinate::Column::G)) {
                this->m_game.board[BoardCoordinate::columnToInt(BoardCoordinate::Column::H) * 8 + this->last_square->getRow()]->setAsMoved();
                std::swap(this->m_game.board[BoardCoordinate::columnToInt(BoardCoordinate::Column::H) * 8 + this->last_square->getRow()], this->m_game.board[BoardCoordinate::columnToInt(BoardCoordinate::Column::F) * 8 + this->last_square->getRow()]);
            } else if (this->moving_piece->getType() == PieceType::Pawn && pressedSquare.getRow() == BoardCoordinate::rowToInt(this->moving_piece->getColor() == PieceColor::White ? 8 : 1)) {
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
                this->moving_piece->promotePawnTo(toPromote);
            } else if (this->moving_piece->getType() == PieceType::Pawn && abs(this->last_square->getRow() - pressedSquare.getRow()) == 2) {
                this->m_game.pawn_double_moved_last_turn = this->moving_piece;
            }
            this->m_game.board[pressedSquare.getCol() * 8 + pressedSquare.getRow()] = this->moving_piece;
            this->moving_piece = nullptr;
            delete this->last_square;
            this->last_square = nullptr;
            m_game.changeTurnColor();
        } else {
            std::swap(this->moving_piece, this->m_game.board[this->last_square->getCol() * 8 + this->last_square->getRow()]);
            delete this->last_square;
            this->last_square = nullptr;
        }
    }
}

void GameUI::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
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
                if (this->m_game.board[i * 8 + j] != nullptr) {
                    const sf::Sprite& currentPieceSprite = this->pieces_sprites[(int)this->m_game.board[i * 8 + j]->getColor() * 6 + (int)this->m_game.board[i * 8 + j]->getType()];
                    sf::Transformable transformer;
                    sf::RenderStates specific_state = state;
                    transformer.setPosition(board_pos_x + i * square_size, board_pos_y + j * square_size);
                    specific_state.transform *= transformer.getTransform();
                    target.draw(currentPieceSprite, specific_state);
                }
            }
        }
        if (this->moving_piece != nullptr) {
            sf::Window* window = dynamic_cast<sf::Window*>(&target);
            if (window != nullptr) {
                const sf::Sprite& currentPieceSprite = this->pieces_sprites[(int)this->moving_piece->getColor() * 6 + (int)this->moving_piece->getType()];
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

GameUI::~GameUI()
{
    if (this->moving_piece != nullptr)
        delete moving_piece;
    if (this->last_square != nullptr)
        delete last_square;
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
    , moving_piece(nullptr)
    , last_square(nullptr)
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
