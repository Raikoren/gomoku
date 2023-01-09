#include "game.hpp"

/* Constructor */
Game::Game() {
    memset(visualData.map, '0', 361);
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
                    else if (!titleScreen) {
                        gaming(ev);
                    }
            }
            visualData.size = size;
            visualData.titleScreen = titleScreen;
            _visual_.getWin()->clear();
            _visual_.draw(visualData);
            _visual_.getWin()->display();
        }
    }
}

void Game::settingUp(sf::Event ev) {
    buttonEvent(&(_visual_.b1), ev, &size, 9);
    buttonEvent(&(_visual_.b2), ev, &size, 13);
    buttonEvent(&(_visual_.b3), ev, &size, 19);
    buttonEvent(&(_visual_.b4), ev, &titleScreen, false);
}

void Game::buttonEvent(Button* b, sf::Event ev, bool* modified, bool modifier) {
    if (b->isTargeted(*(_visual_.getWin()))) {
        if (ev.key.code == sf::Mouse::Left) {
            b->setButtonColor(sf::Color::Red);
            *modified = modifier;
        }
        else if (*modified != modifier) {
            b->setButtonColor(sf::Color::White);
        }
    }
    else if (*modified != modifier) {
        b->setButtonColor(sf::Color::Green);
    }
}

void Game::buttonEvent(Button* b, sf::Event ev, int* modified, int modifier) {
    if (b->isTargeted(*(_visual_.getWin()))) {
        if (ev.key.code == sf::Mouse::Left) {
            b->setButtonColor(sf::Color::Red);
            *modified = modifier;
        }
        else if (*modified != modifier) {
            b->setButtonColor(sf::Color::White);
        }
    }
    else if (*modified != modifier) {
        b->setButtonColor(sf::Color::Green);
    }
}

void Game::gaming(sf::Event ev) {
    double pad = (BOARD - (MIN_MARGIN * 2)) / (size + 1);
    double margin = (BOARD - pad * (size + 1)) / 2;
    int x = 0;
    int y = 0;
    sf::Vector2f mouse(sf::Mouse::getPosition(*(_visual_.getWin())));
    if (targetingBoard(ev, *(_visual_.getWin()), mouse, pad / 2)) {
        x = (mouse.x - (WIN_X / 2 - BOARD / 2 + MIN_MARGIN + pad / 2)) / pad;
        y = (mouse.y - (WIN_Y / 2 - BOARD / 2 + MIN_MARGIN + pad / 2)) / pad;
        x = (x > size - 1) ? size - 1 : x;
        y = (y > size - 1) ? size - 1 : y;
        if (visualData.map[y * size + x] == '0') {
            visualData.previewEnable = true;
            visualData.preview.setFillColor(sf::Color(255, 0, 55, 100));
            visualData.preview.setRadius(30);
            visualData.preview.setPosition(sf::Vector2f((WIN_X / 2 - BOARD / 2) + margin + 7 + (pad / 2) + pad * x,
                (WIN_Y / 2 - BOARD / 2) + margin + 7 + (pad / 2) + pad * y));
            if (ev.key.code == sf::Mouse::Left) {
                turn = (turn) ? false : true;
                visualData.map[y * size + x] = (turn) ? '2' : '1';
                visualData.previewEnable = false;
            }
        }
    }
    else {
        visualData.previewEnable = false;
    }
}

bool Game::targetingBoard(sf::Event, sf::RenderWindow& w , sf::Vector2f m, double p) {
    float bx = WIN_X / 2 - BOARD / 2 + MIN_MARGIN + p;
    float by = WIN_Y / 2 - BOARD / 2 + MIN_MARGIN + p;

    float bxWidth = WIN_X / 2 + BOARD / 2 - MIN_MARGIN - p;
    float byHeight = WIN_Y / 2 + BOARD / 2 - MIN_MARGIN - p;

    if (m.x < bxWidth && m.x > bx && m.y < byHeight && m.y > by)
        return(true);
    else
        return(false);
}