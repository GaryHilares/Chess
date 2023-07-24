#include "../include/GameUI.hpp"

bool GameUI::existInterrumptions(Square source, Square destiny) const
{
    if (source.getCol() == destiny.getCol()) {
        short int modifier = source.getRow() < destiny.getRow() ? 1 : -1;
        for (short int i = source.getRow(); i != destiny.getRow(); i += modifier)
            if (this->readBoard(source.getCol(), i) != nullptr)
                return true;
        return false;
    } else if (source.getRow() == destiny.getRow()) {
        short int modifier = source.getCol() < destiny.getCol() ? 1 : -1;
        for (short int i = source.getCol(); i != destiny.getCol(); i += modifier)
            if (this->game.board[i * 8 + source.getRow()] != nullptr)
                return true;
        return false;
    } else if (abs(source.getRow() - destiny.getRow()) == abs(source.getCol() - destiny.getCol())) {
        short int modifierX = source.getCol() < destiny.getCol() ? 1 : -1;
        short int modifierY = source.getRow() < destiny.getRow() ? 1 : -1;
        for (short int i = source.getCol(), j = source.getRow(); i != destiny.getCol() || j != destiny.getRow(); i += modifierX, j += modifierY)
            if (this->game.board[i * 8 + j] != nullptr)
                return true;
        return false;
    } else {
        return true;
    }
}

Piece* GameUI::readBoard(short int col, short int row) const
{
    if (col > 7 || col < 0 || row > 7 || row < 0)
        return nullptr;
    return this->game.board[col * 8 + row];
}

Piece* GameUI::readBoard(Square target) const
{
    return this->readBoard(target.getCol(), target.getRow());
}

bool GameUI::isLegalMove(const Piece moving, const Square source, const Square destiny) const
{
    if (moving.color != this->game.turn_color)
        return false;
    if (source == destiny || (this->readBoard(destiny.getCol(), destiny.getRow()) != nullptr && moving.color == this->readBoard(destiny.getCol(), destiny.getRow())->color))
        return false;
    const short int movedX = abs(source.getCol() - destiny.getCol());
    const short int movedY = abs(source.getRow() - destiny.getRow());
    const Square kingPosition = [&]() {
        for (short int i = 1; i <= 8; i++) {
            for (short int j = 1; j <= 8; j++) {
                Square cur(static_cast<Square::column>(i), j);
                if (this->readBoard(cur) != nullptr && this->readBoard(cur)->type == Piece::King && this->readBoard(cur)->color == moving.color)
                    return cur;
            }
        }
        return destiny;
    }();
    Piece::Color oposingColor = moving.color == Piece::Black ? Piece::White : Piece::Black;
    for (std::pair<short int, short int> knightPlaces : std::initializer_list<std::pair<short int, short int>> { { -2, -1 }, { -1, -2 }, { -1, 2 }, { 2, -1 }, { 1, 2 }, { 2, 1 }, { 1, -2 }, { -2, 1 } }) {
        if (this->readBoard(kingPosition.getCol() + knightPlaces.first, kingPosition.getRow() + knightPlaces.second) != nullptr && this->readBoard(kingPosition.getCol() + knightPlaces.first, kingPosition.getRow() + knightPlaces.second)->type == Piece::Knight && this->readBoard((kingPosition.getCol() + knightPlaces.first), kingPosition.getRow() + knightPlaces.second)->color == oposingColor && !(destiny.getCol() == kingPosition.getCol() + knightPlaces.first && destiny.getRow() == kingPosition.getRow() + knightPlaces.second)) {
            return false;
        }
    }
    for (std::pair<short int, short int> modifiers : std::initializer_list<std::pair<short int, short int>> { { -1, -1 }, { 1, -1 }, { -1, 1 }, { 1, 1 }, { 0, 1 }, { 0, -1 }, { 1, 0 }, { -1, 0 } }) {
        for (int x = kingPosition.getCol() + modifiers.first, y = kingPosition.getRow() + modifiers.second; x < 8 && x >= 0 && y < 8 && y >= 0; x += modifiers.first, y += modifiers.second) {
            if (this->readBoard(x, y) != nullptr) {
                if (this->readBoard(x, y)->color == oposingColor && !(destiny.getCol() == x && destiny.getRow() == y) && (this->readBoard(x, y)->type == Piece::Queen || ((abs(modifiers.first) != abs(modifiers.second) && this->readBoard(x, y)->type == Piece::Rook) || (abs(modifiers.first) == abs(modifiers.second) && this->readBoard(x, y)->type == Piece::Bishop))))
                    return false;
                else
                    break;
            } else if (destiny.getCol() == x && destiny.getRow() == y) {
                break;
            }
        }
    }
    for (int i : { -1, 1 })
        if (this->readBoard(kingPosition.getCol() + i, kingPosition.getRow() + (oposingColor == Piece::White ? 1 : -1)) != nullptr && this->readBoard(kingPosition.getCol() + i, kingPosition.getRow() + (oposingColor == Piece::White ? 1 : -1))->color == oposingColor && this->readBoard(kingPosition.getCol() + i, kingPosition.getRow() + (oposingColor == Piece::White ? 1 : -1))->type == Piece::Pawn)
            return false;

    switch (moving.type) {
    case Piece::King:
        if (existInterrumptions(source, destiny))
            return false;
        if (destiny.getCol() == Square::columnToInt(Square::column::g) && game.board[Square::columnToInt(Square::column::h) * 8 + source.getRow()] != nullptr && !game.board[Square::columnToInt(Square::column::h) * 8 + source.getRow()]->moved && movedY == 0 && !moving.moved && isLegalMove(moving, source, Square(Square::column::f, moving.color == Piece::White ? 1 : 8)))
            return true;
        if (destiny.getCol() == Square::columnToInt(Square::column::c) && game.board[Square::columnToInt(Square::column::a) * 8 + source.getRow()] != nullptr && !game.board[Square::columnToInt(Square::column::a) * 8 + source.getRow()]->moved && movedY == 0 && !moving.moved && isLegalMove(moving, source, Square(Square::column::d, moving.color == Piece::White ? 1 : 8)))
            return true;
        if (movedX > 1 || movedY > 1)
            return false;
        return true;
    case Piece::Queen:
        if (existInterrumptions(source, destiny))
            return false;
        return true;
    case Piece::Bishop:
        if (movedX != movedY)
            return false;
        if (existInterrumptions(source, destiny))
            return false;
        return true;
    case Piece::Knight:
        if (!((movedX == 2 && movedY == 1) || (movedX == 1 && movedY == 2)))
            return false;
        return true;
    case Piece::Rook:
        if (movedX != 0 && movedY != 0)
            return false;
        if (existInterrumptions(source, destiny))
            return false;
        return true;
    case Piece::Pawn:
        if ((moving.color == Piece::White && Square::rowToInt(source.getRow()) > Square::rowToInt(destiny.getRow())) || (moving.color == Piece::Black && Square::rowToInt(source.getRow()) < Square::rowToInt(destiny.getRow())))
            return false;
        if (existInterrumptions(source, destiny))
            return false;
        if (((moving.color == Piece::White && source.getRow() == Square::rowToInt(2)) || (moving.color == Piece::Black && source.getRow() == Square::rowToInt(7)))
            && movedX == 0 && movedY == 2 && this->game.board[destiny.getCol() * 8 + destiny.getRow()] == nullptr)
            return true;
        else if (movedX == 1 && movedY == 1 && (this->game.board[destiny.getCol() * 8 + destiny.getRow()] != nullptr || (source.getRow() == Square::rowToInt(5) && moving.color == Piece::White && this->game.board[destiny.getCol() * 8 + destiny.getRow() + 1] != nullptr && this->game.board[destiny.getCol() * 8 + destiny.getRow() + 1]->type == Piece::Pawn && this->game.board[destiny.getCol() * 8 + destiny.getRow() + 1] == this->game.pawn_double_moved_last_turn) || (source.getRow() == Square::rowToInt(4) && moving.color == Piece::Black && this->game.board[destiny.getCol() * 8 + destiny.getRow() - 1] != nullptr && this->game.board[destiny.getCol() * 8 + destiny.getRow() - 1]->type == Piece::Pawn && this->game.board[destiny.getCol() * 8 + destiny.getRow() - 1] == this->game.pawn_double_moved_last_turn)))
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
        this->last_square = new Square(getSquare(mousePos.x, mousePos.y));
        std::swap(this->moving_piece, this->game.board[this->last_square->getCol() * 8 + this->last_square->getRow()]);
    } else if ((!sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->moving_piece != nullptr)) {
        Square pressedSquare = getSquare(mousePos.x, mousePos.y);
        if (this->isLegalMove(*this->moving_piece, *this->last_square, pressedSquare)) {
            this->game.pawn_double_moved_last_turn = nullptr;
            this->moving_piece->moved = true;
            if (this->moving_piece->type == Piece::Pawn && this->last_square->getCol() != pressedSquare.getCol() && this->game.board[pressedSquare.getCol() * 8 + pressedSquare.getRow()] == nullptr) {
                if (this->moving_piece->color == Piece::White && this->game.board[pressedSquare.getCol() * 8 + pressedSquare.getRow() + 1] != nullptr) {
                    delete this->game.board[pressedSquare.getCol() * 8 + pressedSquare.getRow() + 1];
                    this->game.board[pressedSquare.getCol() * 8 + pressedSquare.getRow() + 1] = nullptr;
                } else if (this->moving_piece->color == Piece::Black && this->game.board[pressedSquare.getCol() * 8 + pressedSquare.getRow() - 1] != nullptr) {
                    delete this->game.board[pressedSquare.getCol() * 8 + pressedSquare.getRow() - 1];
                    this->game.board[pressedSquare.getCol() * 8 + pressedSquare.getRow() - 1] = nullptr;
                }
            }
            if (this->game.board[pressedSquare.getCol() * 8 + pressedSquare.getRow()] != nullptr)
                delete this->game.board[pressedSquare.getCol() * 8 + pressedSquare.getRow()];
            if (this->moving_piece->type == Piece::King && this->last_square->getCol() == Square::columnToInt(Square::column::e) && pressedSquare.getCol() == Square::columnToInt(Square::column::c)) {
                this->game.board[Square::columnToInt(Square::column::a) * 8 + this->last_square->getRow()]->moved = true;
                std::swap(this->game.board[Square::columnToInt(Square::column::a) * 8 + this->last_square->getRow()], this->game.board[Square::columnToInt(Square::column::d) * 8 + this->last_square->getRow()]);

            } else if (this->moving_piece->type == Piece::King && this->last_square->getCol() == Square::columnToInt(Square::column::e) && pressedSquare.getCol() == Square::columnToInt(Square::column::g)) {
                this->game.board[Square::columnToInt(Square::column::h) * 8 + this->last_square->getRow()]->moved = true;
                std::swap(this->game.board[Square::columnToInt(Square::column::h) * 8 + this->last_square->getRow()], this->game.board[Square::columnToInt(Square::column::f) * 8 + this->last_square->getRow()]);
            } else if (this->moving_piece->type == Piece::Pawn && pressedSquare.getRow() == Square::rowToInt(this->moving_piece->color == Piece::White ? 8 : 1)) {
                Piece::Type toPromote;
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
                    window.clear(moving_piece->color == Piece::White ? sf::Color::White : sf::Color(100, 100, 100));
                    sf::Transformable transformer;
                    sf::RenderStates specificState;
                    transformer.setPosition(100, 350);
                    for (Piece::Type i : { Piece::Queen, Piece::Rook, Piece::Bishop, Piece::Knight }) {
                        transformer.move(100, 0);
                        specificState.transform = transformer.getTransform();
                        window.draw(this->pieces_sprites[moving_piece->color * 6 + i], specificState);
                    }
                    window.display();
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                        sf::Vector2f newMousePosF = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                        sf::Vector2i newMousePos(newMousePosF.x, newMousePosF.y);
                        if (newMousePos.y >= 350 && newMousePos.y <= 450 && newMousePos.x >= 200 && newMousePos.x <= 600) {
                            switch ((newMousePos.x - 200) / 100) {
                            case 0:
                                toPromote = Piece::Queen;
                                break;
                            case 1:
                                toPromote = Piece::Rook;
                                break;
                            case 2:
                                toPromote = Piece::Bishop;
                                break;
                            case 3:
                                toPromote = Piece::Knight;
                                break;
                            case 4:
                                throw std::out_of_range("Out of promotable Pieces");
                            }
                            break;
                        }
                    }
                }
                this->moving_piece->type = toPromote;
            } else if (this->moving_piece->type == Piece::Pawn && abs(this->last_square->getRow() - pressedSquare.getRow()) == 2) {
                this->game.pawn_double_moved_last_turn = this->moving_piece;
            }
            this->game.board[pressedSquare.getCol() * 8 + pressedSquare.getRow()] = this->moving_piece;
            this->moving_piece = nullptr;
            delete this->last_square;
            this->last_square = nullptr;
            if (this->game.turn_color == Piece::White)
                this->game.turn_color = Piece::Black;
            else
                this->game.turn_color = Piece::White;
        } else {
            std::swap(this->moving_piece, this->game.board[this->last_square->getCol() * 8 + this->last_square->getRow()]);
            delete this->last_square;
            this->last_square = nullptr;
        }
    }
}

Piece*& GameUI::accessBoard(short int col, short int row)
{
    if (col > 7 || col < 0 || row > 7 || row < 0)
        throw std::out_of_range("Out of board's range.");
    return this->game.board[col * 8 + row];
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
                if (this->game.board[i * 8 + j] != nullptr) {
                    const sf::Sprite& currentPieceSprite = this->pieces_sprites[this->game.board[i * 8 + j]->color * 6 + this->game.board[i * 8 + j]->type];
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
                const sf::Sprite& currentPieceSprite = this->pieces_sprites[this->moving_piece->color * 6 + this->moving_piece->type];
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

Square GameUI::getSquare(float pos_x, float pos_y)
{
    pos_x = (pos_x - board_pos_x) / square_size;
    pos_y = 8 - ((pos_y - board_pos_y) / square_size);
    int new_pos_x = pos_x + 1;
    int new_pos_y = pos_y + 1;
    return Square(static_cast<Square::column>(new_pos_x), new_pos_y);
}

GameUI::GameUI(GameController& new_game)
    : game(new_game)
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

Piece*& GameUI::accessBoard(Square target)
{
    return this->accessBoard(target.getCol(), target.getRow());
}
