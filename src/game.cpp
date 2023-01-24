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
                    if (!gameOn)
                        settingUp(ev);
                    else if (gameOn) {
                        gaming(ev);
                    }
                default:
                    break;
            }
            visualData.size = size;
            visualData.gameOn = gameOn;
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
    buttonEvent(&(_visual_.b4), ev, &gameOn);
    buttonEvent(&(_visual_.b5), ev, &ko);
    visualData.preview.setRadius((BOARD - (MIN_MARGIN * 2)) / (size + 1) / 2);
}

void Game::buttonEvent(Button* b, sf::Event ev, bool* modified) {
    if (b->isTargeted(*(_visual_.getWin()))) {
        if (ev.mouseButton.button == sf::Mouse::Left) {
            b->setButtonColor(sf::Color::Red);
            *modified = !*modified;
        }
        else if (*modified == false) {
            b->setButtonColor(sf::Color::White);
        }
    }
    else if (*modified == false) {
        b->setButtonColor(sf::Color::Green);
    }
}

void Game::buttonEvent(Button* b, sf::Event ev, int* modified, int modifier) {
    if (b->isTargeted(*(_visual_.getWin()))) {
        if (ev.mouseButton.button == sf::Mouse::Left) {
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
    double pad = (BOARD - (MIN_MARGIN * 2)) / (size - 1);
    double margin = (BOARD - (pad * (size - 1))) / 2;
    int x = 0;
    int y = 0;
    sf::Vector2f mouse(sf::Mouse::getPosition(*(_visual_.getWin())));
    if (!visualData.endGame && !visualData.scoreState && targetingBoard(ev, mouse, margin - pad / 2)) {
        x = (mouse.x - (WIN_X / 2 - BOARD / 2) - pad / 2) / ((BOARD - pad) / size);
        y = (mouse.y - (WIN_Y / 2 - BOARD / 2) - pad / 2) / ((BOARD - pad) / size);
        x = (x >= size) ? size - 1 : x;
        y = (y >= size) ? size - 1 : y;
        if (visualData.map[y * size + x] == '0') {
            visualData.previewEnable = previewToggle ? true : false;
            visualData.preview.setFillColor(sf::Color(255, 0, 55, 100));
            visualData.preview.setPosition(sf::Vector2f((WIN_X / 2 - BOARD / 2) + margin + pad * x - visualData.preview.getRadius(),
                (WIN_Y / 2 - BOARD / 2) + margin + pad * y - visualData.preview.getRadius()));
            if (ev.mouseButton.button == sf::Mouse::Left) {
                memset(territory, '0', 361);
                taking(y * size + x, visualData.map);
                if (!surronded(y * size + x, visualData.map)) {
                    memset(territory, '0', 361);
                    visualData.map[y * size + x] = (turn) ? '2' : '1';
                    visualData.previewEnable = false;
                    turn = (turn) ? false : true;
                }
            }
        }
        else {
            visualData.previewEnable = false;
        }
    }
    else if (visualData.endGame && ev.mouseButton.button == sf::Mouse::Left) {
        visualData.victoryScreen = true;
    }
    else if (!visualData.endGame) {
        visualData.previewEnable = false;
        if (_visual_.b6.isTargeted(*(_visual_.getWin()))) {
            if (ev.mouseButton.button == sf::Mouse::Left) {
                _visual_.b6.setButtonColor(sf::Color::Red);
                pass += 1;
                if (pass == 1) {
                    _visual_.b6.setText("FINISH");
                    _visual_.b6.setButtonColor(sf::Color(255, 255, 0));
                    turn = (turn) ? false : true;
                }
                else {
                    getScore(visualData.map);
                    visualData.endGame = true;
                }
            }
            else {
                _visual_.b6.setButtonColor((pass == 0) ? sf::Color::White : sf::Color(251,206,177));
            }
        }
        buttonEvent(&(_visual_.b7), ev, &hint);
        buttonEvent(&(_visual_.b8), ev, &previewToggle);
        buttonEvent(&(_visual_.b9), ev, &visualData.scoreState);
        if (visualData.scoreState) {
            getScore(visualData.map);
        }
    } 
}

void Game::taking(int pos, char* map) {
    char c = (turn) ? '1' : '2';
    map[pos] = (turn) ? '2' : '1';
    memset(territory, '0', 361);
    bool top = false;
    bool bottom = false;
    bool left = false;
    bool right = false;
    if (pos % size != 0 && map[pos - 1] == c) {
        territory[pos - 1] = c;
        if ((left = surronded(pos - 1, map)))
            for (int i = 0; i < size * size; i++) {
                if (territory[i] != '0') {
                    map[i] = '0';
                }
            }
    }
    memset(territory, '0', 361);
    if (pos >= size && map[pos - size] == c) {
        territory[pos - size] = c;
        if ((top = surronded(pos - size, map)))
            for (int i = 0; i < size * size; i++) {
                if (territory[i] != '0') {
                    map[i] = '0';
                }
            }
    }
    memset(territory, '0', 361);
    if ((pos + 1) % size != 0 && map[pos + 1] == c) {
        territory[pos + 1] = c;
        if ((right = surronded(pos + 1, map)))
            for (int i = 0; i < size * size; i++) {
                if (territory[i] != '0') {
                    map[i] = '0';
                }
            }
    }
    memset(territory, '0', 361);
    if (pos < size * size - size && map[pos + size] == c) {
        territory[pos + size] = c;
        if ((bottom = surronded(pos + size, map)))
            for (int i = 0; i < size * size; i++) {
                if (territory[i] != '0') {
                    map[i] = '0';
                }
            }
    }
    map[pos] = '0';
}

bool Game::targetingBoard(sf::Event, sf::Vector2f m, double p) {
    float bx = (WIN_X / 2 - BOARD / 2) + p;
    float by = (WIN_Y / 2 - BOARD / 2) + p;

    float bxWidth = bx + BOARD - p * 2;
    float byHeight = by + BOARD - p * 2;
    
    if (m.x < bxWidth && m.x > bx && m.y < byHeight && m.y > by)
        return(true);
    else
        return(false);
}

bool Game::surronded(int pos, char* map) {
    if (territory[pos] == '0')
        territory[pos] = (turn) ? '2' : '1';
    bool top = true;
    bool bottom = true;
    bool left = true;
    bool right = true;
    if (pos % size != 0 && territory[pos - 1] != territory[pos]) {
        if (map[pos - 1] == '0')
            return false;
        if (map[pos - 1] == territory[pos]) {
            territory[pos - 1] = territory[pos];
            left = surronded(pos - 1, map);
        }
    }
    if (pos >= size && territory[pos - size] != territory[pos]) {
        if (map[pos - size] == '0')
            return false;
        if (map[pos - size] == territory[pos]) {
            territory[pos - size] = territory[pos];
            top = surronded(pos - size, map);
        }
    }
    if ((pos + 1) % size != 0 && territory[pos + 1] != territory[pos]) {
        if (map[pos + 1] == '0')
            return false;
        if (map[pos + 1] == territory[pos]) {
            territory[pos + 1] = territory[pos];
            right = surronded(pos + 1, map);
        }
    }
    if (pos < size * size - size && territory[pos + size] != territory[pos]) {
        if (map[pos + size] == '0')
            return false;
        if (map[pos + size] == territory[pos]) {
            territory[pos + size] = territory[pos];
            bottom = surronded(pos + size, map);
        }
    }
    return(left && right && top && bottom);
}

void Game::getScore(char* s) {
    memset(visualData.result, '0', 361);
    visualData.bScore = 0;
    visualData.wScore = 0;
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            if (s[y * size + x] == '0') {
                bool surroundedByBlack = false;
                bool surroundedByWhite = false;
                for (int dy = -1; dy <= 1; dy++) {
                    for (int dx = -1; dx <= 1; dx++) {
                        int nx = x + dx;
                        int ny = y + dy;
                        if (nx < 0 || nx >= size || ny < 0 || ny >= size) {
                            continue;
                        }
                        if ((dx == 0 || dy == 0) && s[ny * size + nx] == '1') {
                            surroundedByWhite = true;
                        }
                        if ((dx == 0 || dy == 0) && s[ny * size + nx] == '2') {
                            surroundedByBlack = true;
                        }
                    }
                }
                if (!surroundedByBlack && surroundedByWhite) {
                    visualData.result[y * size + x] = '1';
                    visualData.wScore++;
                }
                if (!surroundedByWhite && surroundedByBlack) {
                    visualData.result[y * size + x] = '2';
                    visualData.bScore++;
                }
            }
        }
    }
}