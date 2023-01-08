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
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                _visual_.getWin()->close();
            }
            switch (ev.type) {
                case sf::Event::Closed:
                    _visual_.getWin()->close();
                case sf::Event::MouseMoved:
                case sf::Event::MouseButtonReleased:
                    if (titleScreen)
                        settingUp(ev);
            }
            _visual_.getWin()->clear();
            _visual_.draw(titleScreen, size);
            _visual_.getWin()->display();
        }
    }
}

void Game::settingUp(sf::Event ev) {
    if (_visual_.b1.isTargeted(*(_visual_.getWin()))) {
        if (ev.key.code == sf::Mouse::Left) {
            _visual_.b1.setButtonColor(sf::Color::Red);
            size = 9;
        }
        else if (size != 9) {
            _visual_.b1.setButtonColor(sf::Color::White);
        }
    }
    else if (size != 9) {
        _visual_.b1.setButtonColor(sf::Color::Green);
    }
    if (_visual_.b2.isTargeted(*(_visual_.getWin()))) {
        if (ev.key.code == sf::Mouse::Left) {
            _visual_.b2.setButtonColor(sf::Color::Red);
            size = 13;
        }
        else if (size != 13) {
            _visual_.b2.setButtonColor(sf::Color::White);
        }
    }
    else if (size != 13) {
        _visual_.b2.setButtonColor(sf::Color::Green);
    }
    if (_visual_.b3.isTargeted(*(_visual_.getWin()))) {
        if (ev.key.code == sf::Mouse::Left) {
            _visual_.b3.setButtonColor(sf::Color::Red);
            size = 19;
        }
        else if (size != 19) {
            _visual_.b3.setButtonColor(sf::Color::White);
        }
    }
    else if (size != 19) {
        _visual_.b3.setButtonColor(sf::Color::Green);
    }
    if (_visual_.b4.isTargeted(*(_visual_.getWin()))) {
        if (ev.key.code == sf::Mouse::Left) {
            titleScreen = false;
        }
        _visual_.b4.setButtonColor(sf::Color::White);
    }
    else {
        _visual_.b4.setButtonColor(sf::Color::Green);
    }
}