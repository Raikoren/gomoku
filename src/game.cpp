#include "game.hpp"

/* Constructor */
Game::Game() {

}

/* Destructor */
Game::~Game() {

}

/* Run function */
void Game::run() {
    while (_visual_.getWin()->isOpen())
    {
        sf::Event ev;
        while (_visual_.getWin()->pollEvent(ev)) {
            if (ev.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                _visual_.getWin()->close();
        }
        _visual_.getWin()->clear();
        _visual_.draw();
        _visual_.getWin()->display();
    }
}