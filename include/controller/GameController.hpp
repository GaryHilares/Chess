#pragma once
#include "../GameUI.hpp"
#include "../model/GameState.hpp"
#include <SFML/Graphics.hpp>

class GameController {
    sf::RenderWindow m_window;
    GameState m_game;
    GameUI m_view;

public:
    GameController();
    void run();
};