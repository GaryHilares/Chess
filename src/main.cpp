#include "../include/Piece.hpp"
#include "../include/Square.hpp"
#include "../include/GameController.hpp"
#include "../include/GameUI.hpp"
#include <SFML/Graphics.hpp>

//ISSUE: O-O in check.

int main()
{
    sf::RenderWindow window(sf::VideoMode(800,800),"Chess1");
    window.setFramerateLimit(60);
    bool focused = true;
    GameController game;
    GameUI ui(game);
    while(window.isOpen())
    {
        sf::Event event;
        if(window.pollEvent(event))
        {
            switch(event.type)
            {
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
        if(focused)
        {
            ui.update(window);
            window.clear(sf::Color::Magenta);
            window.draw(ui);
            window.display();
        }

    }
}
