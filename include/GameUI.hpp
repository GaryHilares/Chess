#pragma once
#include "model/GameState.hpp"
#include <SFML/Graphics.hpp>
#include <optional>

class GameUI : public sf::Drawable {
private:
    static constexpr int board_pos_x = 0;
    static constexpr int board_pos_y = 0;
    static constexpr int square_size = 70;
    static constexpr int piece_size_in_texture = 200;
    std::array<sf::Sprite, 12> pieces_sprites;
    GameState& m_game;
    std::optional<BoardCoordinate> last_square;
    sf::Texture pieces_texture;

public:
    GameUI(GameState& new_game);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
    BoardCoordinate fromUiCoordsToBoardCoords(float pos_x, float pos_y);
    void update(sf::RenderWindow& window);
};
