#pragma once
#include "model/GameState.hpp"
#include <SFML/Graphics.hpp>

class GameUI : public sf::Drawable {
private:
    static constexpr int board_pos_x = 0;
    static constexpr int board_pos_y = 0;
    static constexpr int square_size = 70;
    static constexpr int piece_size_in_texture = 200;
    std::array<sf::Sprite, 12> pieces_sprites;
    GameState& m_game;
    Piece* moving_piece;
    Square* last_square;
    sf::Texture pieces_texture;
    bool isLegalMove(const Piece moving, const Square source, const Square destiny) const;

public:
    GameUI(GameState& new_game);
    ~GameUI();
    virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
    Square getSquare(float pos_x, float pos_y);
    void update(sf::RenderWindow& window);
};
