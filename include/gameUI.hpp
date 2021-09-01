#pragma once
#include "gameController.hpp"
#include "square.hpp"
#include <SFML/Graphics.hpp>

class GameUI: public sf::Drawable
{
private:
    static constexpr int board_pos_x = 0;
    static constexpr int board_pos_y = 0;
    static constexpr int square_size = 100;
    static constexpr int piece_size_in_texture = 200;
    std::array<sf::Sprite,12> pieces_sprites;
    GameController& game;
    Piece* moving_piece;
    Square* last_square;
    sf::Texture pieces_texture;
    bool existInterrumptions(Square source, Square destiny) const;
    bool isLegalMove(const Piece moving, const Square source, const Square destiny) const;
    Piece*& accessBoard(short int col, short int row);
    Piece*& accessBoard(Square target);
    Piece* readBoard(short int col, short int row) const;
    Piece* readBoard(Square target) const;
public:
    GameUI(GameController& new_game);
    ~GameUI();
    virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
    Square getSquare(float pos_x, float pos_y);
    void update(sf::RenderWindow& window);
};
