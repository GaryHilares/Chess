#include "../../include/controller/GameController.hpp"

GameController::GameController()
    : m_window(sf::VideoMode(800, 800), "New Game - Chess.cpp")
    , m_game()
    , m_view(m_game)
{
    m_window.setFramerateLimit(60);
}

void GameController::run()
{
    // Run loop until the window is closed
    while (m_window.isOpen()) {
        // Event loop
        sf::Event event;
        if (m_window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                m_window.close();
                break;
            default:
                break;
            }
        }

        // Update the window only if it is focused
        if (m_window.hasFocus()) {
            m_view.update(m_window);
            m_window.clear(sf::Color::Magenta);
            m_window.draw(m_view);
            m_window.display();
        }
    }
}