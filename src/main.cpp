/**
 * @file main.cpp
 * @brief Declares main.cpp, the entrypoint of Chess.cpp.
 */
#include "../include/GameUI.hpp"
#include "../include/Square.hpp"
#include "../include/model/GameState.hpp"
#include "../include/model/Piece.hpp"
#include <SFML/Graphics.hpp>

/**
 * @brief Entrypoint of the program. Opens a window running chess and iterates over its event loop.
 * @return Returns 0 unless there is an error.
 */
int main()
{
    // Open and configure the window
    sf::RenderWindow window(sf::VideoMode(800, 800), "New Game - Chess.cpp");
    window.setFramerateLimit(60);

    // Set initial values
    bool focused = true;
    GameState game;
    GameUI ui(game);

    // Run loop until the window is closed
    while (window.isOpen()) {
        // Event loop
        sf::Event event;
        if (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::LostFocus:
                focused = false;
                break;
            case sf::Event::GainedFocus:
                focused = true;
                break;
            default:
                break;
            }
        }

        // Update the window only if it is focused
        if (focused) {
            ui.update(window);
            window.clear(sf::Color::Magenta);
            window.draw(ui);
            window.display();
        }
    }
}
