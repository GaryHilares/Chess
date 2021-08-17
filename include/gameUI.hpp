#pragma once
#include "gameController.hpp"
#include "square.hpp"
#include <SFML/Graphics.hpp>

class gameUI: public sf::Drawable
{
private:
    static constexpr int boardPosX = 0;
    static constexpr int boardPosY = 0;
    static constexpr int squareSize = 100;
    static constexpr int pieceSizeInTexture = 200;
    std::array<sf::Sprite,12> piecesSprites;
    gameController& game;
    piece* movingPiece;
    square* lastSquare;
    sf::Texture piecesTexture;
    bool existInterrumptions(square source, square destiny) const;
    bool isLegalMove(const piece moving, const square source, const square destiny) const;
    piece*& accessBoard(short int col, short int row);
    piece*& accessBoard(square target);
    piece* readBoard(short int col, short int row) const;
    piece* readBoard(square target) const;
public:
    gameUI(gameController& _game);
    ~gameUI();
    virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
    square getSquare(float posX, float posY);
    void update(sf::RenderWindow& window);
};
