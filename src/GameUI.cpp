#include "../include/GameUI.hpp"

void GameUI::update(sf::RenderWindow& window)
{
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    if ((sf::Mouse::isButtonPressed(sf::Mouse::Left) && !this->last_square.has_value())) {
        // If not grabbing piece, pick up piece if it has started moving
        this->last_square = fromUiCoordsToBoardCoords(mousePos.x, mousePos.y);
    } else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->last_square.has_value()) {
        // If piece is dropped, try to move it to current square.
        BoardCoordinate pressed_square = fromUiCoordsToBoardCoords(mousePos.x, mousePos.y);
        if (m_game.isLegalMove(this->last_square.value(), pressed_square)) {
            m_game.move(this->last_square.value(), pressed_square);
        }
        this->last_square.reset();
    }
}

void GameUI::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
    std::optional<Piece> moving_piece = std::nullopt;
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
        for (unsigned int i = 1; i <= 8; i++) {
            for (unsigned int j = 1; j <= 8; j++) {
                if (m_game.readBoard(i, j).has_value() && this->m_game.readBoard(i, j) != moving_piece) {
                    const sf::Sprite& currentPieceSprite = this->pieces_sprites[(int)this->m_game.readBoard(i, j)->getColor() * 6 + (int)this->m_game.readBoard(i, j)->getType()];
                    sf::Transformable transformer;
                    sf::RenderStates specific_state = state;
                    transformer.setPosition(board_pos_x + (i - 1) * square_size, board_pos_y + (8 - j) * square_size);
                    specific_state.transform *= transformer.getTransform();
                    target.draw(currentPieceSprite, specific_state);
                }
            }
        }
        if (moving_piece.has_value()) {
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

BoardCoordinate GameUI::fromUiCoordsToBoardCoords(float pos_x, float pos_y)
{
    pos_x = (pos_x - board_pos_x) / square_size;
    pos_y = 8 - ((pos_y - board_pos_y) / square_size);
    int new_pos_y = pos_y + 1;
    switch ((int)pos_x) {
    case 0:
        return BoardCoordinate(BoardColumn::A, new_pos_y);
    case 1:
        return BoardCoordinate(BoardColumn::B, new_pos_y);
    case 2:
        return BoardCoordinate(BoardColumn::C, new_pos_y);
    case 3:
        return BoardCoordinate(BoardColumn::D, new_pos_y);
    case 4:
        return BoardCoordinate(BoardColumn::E, new_pos_y);
    case 5:
        return BoardCoordinate(BoardColumn::F, new_pos_y);
    case 6:
        return BoardCoordinate(BoardColumn::G, new_pos_y);
    case 7:
        return BoardCoordinate(BoardColumn::H, new_pos_y);
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
