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
                    if (!goOn && !gomokuOn)
                        settingUp(ev);
                    else {
                        gaming(ev);
                    }
                default:
                    break;
            }
            visualData.size = size;
            if (goOn || gomokuOn) {
                visualData.gameOn = true;
                visualData.gomoku = (gomokuOn) ? true : false;
            }
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
    if (_visual_.b4.isTargeted(*(_visual_.getWin()))) {
        _visual_.b4.setButtonTexture(_visual_.getT7());
        gomokuOn = (ev.mouseButton.button == sf::Mouse::Left) ? true : false;
    }
    else
        _visual_.b4.setButtonTexture(_visual_.getT6());
    if (_visual_.b5.isTargeted(*(_visual_.getWin()))) {
        _visual_.b5.setButtonTexture(_visual_.getT5());
        goOn = (ev.mouseButton.button == sf::Mouse::Left) ? true : false;
    }
    else
        _visual_.b5.setButtonTexture(_visual_.getT4());
    if (gomokuOn || goOn) {
        _visual_.setGoban(size);
        _visual_.bPound.setRadius((BOARD - (MIN_MARGIN * 2)) / (size + 1) / 2);
        _visual_.wPound.setRadius((BOARD - (MIN_MARGIN * 2)) / (size + 1) / 2);
        visualData.preview.setRadius((BOARD - (MIN_MARGIN * 2)) / (size + 1) / 2);
    }
}

void Game::buttonEvent(Button* b, sf::Event ev, bool* modified) {
    if (b->isTargeted(*(_visual_.getWin()))) {
        if (ev.mouseButton.button == sf::Mouse::Left) {
            b->setButtonTexture(_visual_.getT3());
            *modified = !*modified;
        }
        else if (*modified == false) {
            b->setButtonTexture(_visual_.getT2());
        }
    }
    else if (*modified == false) {
        b->setButtonTexture(_visual_.getT1());
    }
}

void Game::buttonEvent(Button* b, sf::Event ev, int* modified, int modifier) {
    if (b->isTargeted(*(_visual_.getWin()))) {
        if (ev.mouseButton.button == sf::Mouse::Left) {
            b->setButtonTexture(_visual_.getT3());
            *modified = modifier;
        }
        else if (*modified != modifier) {
            b->setButtonTexture(_visual_.getT2());
        }
    }
    else if (*modified != modifier) {
        b->setButtonTexture(_visual_.getT1());
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
                if (goOn) {
                    memset(territory, '0', 361);
                    taking(y * size + x, visualData.map);
                    if (!surronded(y * size + x, visualData.map)) {
                        memset(territory, '0', 361);
                        visualData.map[y * size + x] = (turn) ? '2' : '1';
                        visualData.previewEnable = false;
                        turn = (turn) ? false : true;
                    }
                }
                else if (gomokuOn) {
                    if (!doubleThreeDetector(y * size + x, visualData.map, (turn) ? '2' : '1')) {
                        visualData.previewEnable = false;
                        mokuVictory(x, y);
                        turn = (turn) ? false : true;
                    }
                }
            }
        }
        else {
            visualData.previewEnable = false;
        }
    }
    else if (visualData.victoryScreen && ev.mouseButton.button == sf::Mouse::Left) {
        resetGame();
    }
    else if (visualData.endGame && ev.mouseButton.button == sf::Mouse::Left) {
        visualData.victoryScreen = true;
    }
    else if (!visualData.endGame) {
        visualData.previewEnable = false;
        if (_visual_.b6.isTargeted(*(_visual_.getWin()))) {
            if (ev.mouseButton.button == sf::Mouse::Left) {
                _visual_.b6.setButtonTexture(_visual_.getT3());
                pass += 1;
                if (pass == 1) {
                    _visual_.b6.setText("FINISH");
                    _visual_.b6.setButtonTexture(_visual_.getT1());
                    turn = (turn) ? false : true;
                }
                else {
                    getScore(visualData.map);
                    visualData.endGame = true;
                }
            }
            else {
                _visual_.b6.setButtonTexture(_visual_.getT2());
            }
        }
        if (_visual_.bReturn.isTargeted(*(_visual_.getWin()))) {
            _visual_.bReturn.setButtonTexture(_visual_.getT3());
            if (ev.mouseButton.button == sf::Mouse::Left) {
                resetGame();
            }
        }
        else
            _visual_.bReturn.setButtonTexture(_visual_.getT1());
        buttonEvent(&(_visual_.b7), ev, &hint);
        buttonEvent(&(_visual_.b8), ev, &previewToggle);
        buttonEvent(&(_visual_.b9), ev, &visualData.scoreState);
        if (visualData.scoreState) {
            getScore(visualData.map);
        }
    } 
}
void Game::resetGame() {
    visualData.bScore = 0;
    visualData.wScore = 0;
    visualData.gameOn = false;
    visualData.gomoku = false;
    visualData.endGame = false;
    visualData.scoreState = false;
    visualData.victoryScreen = false;
    turn = true;
    pass = 0;
    hint = false;
    lastTurn = false;
    previewToggle = true;
    gomokuOn = false;
    goOn = false;
    memset(visualData.map, '0', 361);
}

void Game::mokuVictory(int x, int y) {
    char p = visualData.map[y * size + x];
    int dx = -1;
    int dy = -1;
    double pad = (BOARD - (MIN_MARGIN * 2)) / (size - 1);
    double margin = (BOARD - (pad * (size - 1))) / 2;

    if (visualData.bScore == 5 || visualData.wScore == 5) {
        visualData.endGame = true;
        return;
    }
    if (lastTurn) {
        for (int i = 0; i <= 4; i++) {
            if (visualData.map[wLine[i + 5] * size + wLine[i]] == '0') {
                lastTurn = false;
                break;
            }
        }
        if (lastTurn) {
            visualData.endGame = true;
            visualData.wScore = (turn) ? 999 : 0;
            visualData.bScore = (turn) ? 0 : 999;
        }
    }
    if (fivePound(-1, -1, x, y, p))
        moku[2] = 1;
    else if (fivePound(0, -1, x, y, p)) {
        moku[2] = 2;
        dx = 0;
    }
    else if (fivePound(1, -1, x, y, p)) {
        moku[2] = 3;
        dx = 1;
    }
    else if (fivePound(-1, 0, x, y, p)) {
        moku[2] = 4;
        dy = 0;
    }
    else
        return;
    visualData.endGame = true;
    visualData.line[0] = sf::Vertex(sf::Vector2f((WIN_X / 2 - BOARD / 2) + margin + pad * moku[0], (WIN_Y / 2 - BOARD / 2) + margin + pad * moku[1]),
        sf::Color::Green);
    visualData.line[1] = sf::Vertex(sf::Vector2f((WIN_X / 2 - BOARD / 2) + margin + pad * moku[0] + ((pad * 4) * (dx * -1)), (WIN_Y / 2 - BOARD / 2) + margin + pad * moku[1] + ((pad * 4) * (dy * -1))),
        sf::Color::Green);
    if ((turn && visualData.bScore < 4) || (!turn && visualData.wScore < 4)) {
        for (int i = 0; i <= 4; i++) {
            wLine[i] = moku[0] - (dx * i);
            wLine[i + 5] = moku[1] - (dy * i);
            if (vulnerable(moku[0] - (dx * i), moku[1] - (dy * i), p)) {
                lastTurn = true;
                visualData.endGame = false;
            }
        }
    }
    else if ((turn && visualData.bScore == 4) || (!turn && visualData.wScore == 4))
        for (int i = 0; i < size * size; i++) {
            if (visualData.map[i] == p && vulnerable(i % size, i / size, p)) {
                lastTurn = true;
                visualData.endGame = false;
            }
        }
    if (visualData.endGame) {
        visualData.bScore = (turn) ? 999 : 0;
        visualData.wScore = (turn) ? 0 : 999;
    }
}

// Il est tres tard donc pas de jugement svp
// TODO: rendre ca clean parce que ca fait 1h que je passe et repasse sur cette merde et j'ai honte de zinzin

bool Game::vulnerable(int x, int y, char p) {
    char e = (p == '1') ? '2' : '1';
    if ((x - 2 >= 0 && y - 2 >= 0 && x + 1 < size && y + 1 < size && visualData.map[(y - 1) * size + x - 1] == p && visualData.map[(y - 2) * size + x - 2] == e && visualData.map[(y + 1) * size + x + 1] == '0')
        || (x - 2 >= 0 && y - 2 >= 0 && x + 1 < size && y + 1 < size && visualData.map[(y - 1) * size + x - 1] == p && visualData.map[(y - 2) * size + x - 2] == '0' && visualData.map[(y + 1) * size + x + 1] == e)
        || (x - 1 >= 0 && y - 1 >= 0 && x + 2 < size && y + 2 < size && visualData.map[(y + 1) * size + x + 1] == p && visualData.map[(y + 2) * size + x + 2] == e && visualData.map[(y - 1) * size + x - 1] == '0') 
        || (x - 1 >= 0 && y - 1 >= 0 && x + 2 < size && y + 2 < size && visualData.map[(y + 1) * size + x + 1] == p && visualData.map[(y + 2) * size + x + 2] == '0' && visualData.map[(y - 1) * size + x - 1] == e)
        || (x - 1 >= 0 && y - 2 >= 0 && x + 2 < size && y + 1 < size && visualData.map[(y - 1) * size + x + 1] == p && visualData.map[(y - 2) * size + x + 2] == e && visualData.map[(y + 1) * size + x - 1] == '0')
        || (x - 1 >= 0 && y - 2 >= 0 && x + 2 < size && y + 1 < size && visualData.map[(y - 1) * size + x + 1] == p && visualData.map[(y - 2) * size + x + 2] == '0' && visualData.map[(y + 1) * size + x - 1] == e)
        || (x - 2 >= 0 && y - 1 >= 0 && x + 1 < size && y + 2 < size && visualData.map[(y + 1) * size + x - 1] == p && visualData.map[(y + 2) * size + x - 2] == e && visualData.map[(y + 1) * size + x + 1] == '0')
        || (x - 2 >= 0 && y - 1 >= 0 && x + 1 < size && y + 2 < size && visualData.map[(y + 1) * size + x - 1] == p && visualData.map[(y + 2) * size + x - 2] == '0' && visualData.map[(y + 1) * size + x + 1] == e)
        || (y + 2 < size && y - 1 >= 0 && visualData.map[(y + 1) * size + x] == p && visualData.map[(y + 2) * size + x] == '0' && visualData.map[(y - 1) * size + x] == e)
        || (y + 2 < size && y - 1 >= 0 && visualData.map[(y + 1) * size + x] == p && visualData.map[(y + 2) * size + x] == e && visualData.map[(y - 1) * size + x] == '0')
        || (y + 1 < size && y - 2 >= 0 && visualData.map[(y - 1) * size + x] == p && visualData.map[(y - 2) * size + x] == '0' && visualData.map[(y + 1) * size + x] == e)
        || (y + 1 < size && y - 2 >= 0 && visualData.map[(y - 1) * size + x] == p && visualData.map[(y - 2) * size + x] == e && visualData.map[(y + 1) * size + x] == '0')
        || (x + 2 < size && x - 1 >= 0 && visualData.map[y * size + x + 1] == p && visualData.map[y * size + x + 2] == '0' && visualData.map[y * size + x - 1] == e)
        || (x + 2 < size && x - 1 >= 0 && visualData.map[y * size + x + 1] == p && visualData.map[y * size + x + 2] == e && visualData.map[y * size + x - 1] == '0')
        || (x + 1 < size && x - 2 >= 0 && visualData.map[y * size + x - 1] == p && visualData.map[y * size + x - 2] == '0' && visualData.map[y * size + x + 1] == e)
        || (x + 1 < size && x - 2 >= 0 && visualData.map[y * size + x - 1] == p && visualData.map[y * size + x - 2] == e && visualData.map[y * size + x + 1] == '0')
        )
        return true;
    return(false);
}

bool Game::fivePound(int dx, int dy, int x, int y, char p) {
    int nx = x;
    int ny = y;
    int i = 0;
    int j = 1;
    int hl[2];
    while (nx >= 0 && nx < size && ny >= 0 && ny < size && visualData.map[ny * size + nx] == p && i < 5) {
        hl[0] = nx;
        hl[1] = ny;
        nx += dx;
        ny += dy;
        i++;
    }
    nx = x - dx;
    ny = y - dy;
    while (nx >= 0 && nx < size && ny >= 0 && ny < size && visualData.map[ny * size + nx] == p && i < 5) {
        nx -= dx;
        ny -= dy;
        i++;
        j++;
    }
    if (i == 5) {
        moku[0] = hl[0];
        moku[1] = hl[1];
        return(true);
    }
    return(false);
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

bool Game::doubleThreeDetector(int pos, char* map, char p) {
    map[pos] = p;
    char e = (p == '1') ? '2' : '1';
    bool threeLine = false;
    bool forbiden = false;
    bool took = false;
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            int nx = pos % size + dx;
            int ny = pos / size + dy;
            if (nx < 0 || nx >= size || ny < 0 || ny >= size) {
                continue;
            }
            if (map[ny * size + nx] == e && mokuTake(dx, dy, pos % size, pos / size, map)) {
                map[ny * size + nx] = '0';
                map[ny * size + size * dy + nx + dx] = '0';
                visualData.bScore += (turn) ? 1 : 0;
                visualData.wScore += (turn) ? 0 : 1;
                if (visualData.bScore == 5 || visualData.wScore == 5)
                    visualData.endGame = true;
                took = true;
            }
            if (!took && threeLineDetector(dx, dy, pos % size, pos / size, map)) {
                if (threeLine)
                    forbiden = true;
                else
                    threeLine = true;
            }
        }
    }
    map[pos] = (forbiden && !took) ? '0' : p;
    return(forbiden && !took);
}

bool Game::mokuTake(int dx, int dy, int x, int y, char* m) {
    if ((x + dx * 3 >= 0 && x + dx * 3 < size && y + dy * 3 >= 0 && y + dy * 3< size)
        && m[(y * size + ((dy * size) * 2)) + x + (dx * 2)] != m[y * size + x]
        && m[(y * size + ((dy * size) * 2)) + x + (dx * 2)] != '0'
        && m[(y * size + ((dy * size) * 3)) + x + (dx * 3)] == m[y * size + x]) {
        return (true);
    }
    return(false);
}

bool Game::threeLineDetector(int dx, int dy, int x, int y, char* map) {
    char    p = map[y * size + x];
    char    e = (p == '1') ? '2' : '1';
    int     hole = 0;
    int     ally = 0;
    bool    ennemy = false;
    int     epos;

    for (int i = 1; i < 5; i++) {
        if ((x + dx * i < 0 && x + dx * i >= size && y + dy * i < 0 && y + dy * i >= size)
        || map[(y * size + ((dy * size) * i)) + x + (dx * i)] == e) {
            ennemy = true;
            epos = i - 1;
            break;
        }
        if (map[(y * size + ((dy * size) * i)) + x + (dx * i)] == '0') {
            hole++;
            if (ally == 2 || hole == 2)
                break;
        }
        if (map[(y * size + ((dy * size) * i)) + x + (dx * i)] == p) {
            if (ally == 2)
                return false;
            ally++;
        }
    }
    if (ennemy && map[(y * size + ((dy * size) * epos)) + x + (dx * epos)] == p)
        return(false);
    dy *= -1;
    dx *= -1;
    if (ally == 2) {
        if (map[(y * size + (dy * size)) + x + dx] == '0')
            return(true);
        return (false);
    }
    if (ally == 1 && map[(y * size + (dy * size)) + x + dx] == p && map[(y * size + ((dy * size) * 2)) + x + (dx * 2)] == '0') {
        if (hole == 2 || (hole == 1 && (dy == -1 || (dy == 0 && dx == -1))))
            return(true);
    }
    return false;
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