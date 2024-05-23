/**
 * @file main.cpp
 * @brief Declares main.cpp, the entrypoint of Chess.cpp.
 */
#include "../include/controller/GameController.hpp"
#include <SFML/Graphics.hpp>

/**
 * @brief Entrypoint of the program. Opens a window running chess and iterates over its event loop.
 * @return Returns 0 unless there is an error.
 */
int main()
{
    GameController game;
    game.run();
}
