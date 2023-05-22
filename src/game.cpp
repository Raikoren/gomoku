#include "game.hpp"

/* Constructor */
Game::Game() {
    memset(visualData.map, '0', 361);
    visualData.preview.setFillColor(sf::Color(255, 0, 55, 100));
    visualData.hint.setFillColor(sf::Color(0, 0, 255, 100));
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
                return;
            }
            switch (ev.type) {
                case sf::Event::Closed:
                    _visual_.getWin()->close();
                    return;
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
            algoData.size = size;
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

// Fonction qui g�res les event de l'ecran titre 
void Game::settingUp(sf::Event ev) {
    buttonEvent(&(_visual_.b1), ev, &size, 9);
    buttonEvent(&(_visual_.b2), ev, &size, 13);
    buttonEvent(&(_visual_.b3), ev, &size, 19);
    buttonEvent(&(_visual_.bVsAi), ev, &vsAi);
    if (_visual_.b4.isTargeted(*(_visual_.getWin()))) {
        _visual_.b4.setButtonTexture(_visual_.getT7());
        gomokuOn = (ev.mouseButton.button == sf::Mouse::Left) ? true : false;
    }
    else
        _visual_.b4.setButtonTexture(_visual_.getT6());
    if (_visual_.b5.isTargeted(*(_visual_.getWin()))) {
        _visual_.b5.setButtonTexture(_visual_.getT5());
        goOn = (ev.mouseButton.button == sf::Mouse::Left) ? true : false;
        vsAi = goOn ? false : vsAi;
    }
    else
        _visual_.b5.setButtonTexture(_visual_.getT4());
    if (gomokuOn || goOn) {
        _visual_.setGoban(size);
        _visual_.bPound.setRadius((BOARD - (MIN_MARGIN * 2)) / (size + 6) / 2);
        _visual_.wPound.setRadius((BOARD - (MIN_MARGIN * 2)) / (size + 6) / 2);
        visualData.preview.setRadius((BOARD - (MIN_MARGIN * 2)) / (size + 1) / 2);
        visualData.hint.setRadius((BOARD - (MIN_MARGIN * 2)) / (size + 1) / 2);
    }
}

// g�res les animation des boutons et toggle les boolean qui leur sont reli�s
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

// same fonction mais change un int � un autre int (utilis� leggit que pour la size de goban)
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

// Fonction qui g�re les event en jeu (go, gomoku et ecran de fin)
void Game::gaming(sf::Event ev) {
    double pad = (BOARD - (MIN_MARGIN * 2)) / (size - 1);
    double margin = (BOARD - (pad * (size - 1))) / 2;
    int x = 0;
    int y = 0;
    sf::Vector2f mouse(sf::Mouse::getPosition(*(_visual_.getWin())));
    if (!visualData.endGame && !visualData.scoreState && targetingBoard(ev, mouse, margin - pad / 2)) { // si on est en jeu et que la souris est sur le board
        x = (mouse.x - (WIN_X / 2 - BOARD / 2) - pad / 2) / ((BOARD - pad) / size);
        y = (mouse.y - (WIN_Y / 2 - BOARD / 2) - pad / 2) / ((BOARD - pad) / size);
        x = (x >= size) ? size - 1 : x;
        y = (y >= size) ? size - 1 : y;
        if (vsAi && !turn) {
            algoData.wScore = visualData.wScore;
            algoData.bScore = visualData.bScore;
            memcpy(algoData.map, visualData.map, 361);
            algoData.turn = turn;
			int minMaxRes = algo.ask(algoData);
            if (lastTurn)
                minMaxRes = savingMove;
            !doubleThreeDetector(minMaxRes, visualData.map, '1');
            mokuVictory(minMaxRes % size, minMaxRes / size);
            turn = !turn;
            visualData.totalTurn++;
            _visual_.getGameState()->setString("C'est le tour des NOIRS ^^");
        }
        if (visualData.map[y * size + x] == '0') { // si la position cibl�e est vide
            visualData.previewEnable = previewToggle ? true : false;
            visualData.preview.setPosition(sf::Vector2f((WIN_X / 2 - BOARD / 2) + margin + pad * x - visualData.preview.getRadius(),
                (WIN_Y / 2 - BOARD / 2) + margin + pad * y - visualData.preview.getRadius()));
            if (ev.mouseButton.button == sf::Mouse::Left) { // clique gauche
                if (goOn) { // ruleset du go
                    memset(territory, '0', 361);
                    taking(y * size + x, visualData.map); // Si il y a une capture, on l'enregistre car le move ne pourras pas etre interdit
                    if (!surronded(y * size + x, visualData.map)) { // on v�rifie si il s'agit d'un suicide pour eviter les suicide
                        memset(territory, '0', 361);
                        visualData.map[y * size + x] = (turn) ? '2' : '1';
                        visualData.previewEnable = false;
                        turn = (turn) ? false : true;
                    }
                }
                else if (gomokuOn) { // ruleset du go
                    if (!doubleThreeDetector(y * size + x, visualData.map, (turn) ? '2' : '1')) { // v�rifie l'introduction d'une double three
                        visualData.previewEnable = false;
                        visualData.hintOn = false;
                        mokuVictory(x, y); // V�rifie les diverse condition de victoire
                        turn = !turn;
                        _visual_.getGameState()->setString(turn ? "C'est le tour des NOIRS ^^" : "C'est le tour des BLANCS ^^");
                        if (vsAi)
                            _visual_.getGameState()->setString("Att je reflechie la xD");
                        visualData.totalTurn++;
                    }
                }
            }
        }
        else { // enl�ve la preview si souris hors du goban
            visualData.previewEnable = false;
        }
    }
    else if (visualData.victoryScreen && ev.mouseButton.button == sf::Mouse::Left) { // retour � l'ecran titre avec un clique dans l'ecran de victoire
        resetGame();
    }
    else if (visualData.endGame && ev.mouseButton.button == sf::Mouse::Left) { // montre les condition de victoire pour les deux jeux avant de mettre l'ecran de victoire
        visualData.victoryScreen = true;
    }
    else if (!visualData.endGame) { // g�res les boutons en dehors du goban
        visualData.previewEnable = false;
        if (goOn && _visual_.b6.isTargeted(*(_visual_.getWin()))) { // g�res le bouton pour passer et fini les game de Go
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
        if (_visual_.bReturn.isTargeted(*(_visual_.getWin()))) { // g�re le bouton de retour � l'�cran titre
            _visual_.bReturn.setButtonTexture(_visual_.getT3());
            if (ev.mouseButton.button == sf::Mouse::Left) {
                resetGame();
                _visual_.bReturn.setButtonTexture(_visual_.getT1());
            }
        }
        else
            _visual_.bReturn.setButtonTexture(_visual_.getT1());
        // le tern�re gere les bouton de hint (par l'algo) et de preview de score pour le Go
        buttonEvent(goOn ? &(_visual_.b9) : &(_visual_.b7), ev, goOn ? &visualData.scoreState : &hint); // TODO: setup visualData.hint avec l'algo (meme system que preview, un pion transparent en plus qu'on place et qui montre le hint)
		if (hint == true) {
            algoData.wScore = visualData.wScore;
            algoData.bScore = visualData.bScore;
            memcpy(algoData.map,visualData.map, 361);
            algoData.turn = turn;
            visualData.hintOn = true;
            int res = algo.ask(algoData);
            visualData.hintOn = true;
            visualData.hint.setPosition(sf::Vector2f((WIN_X / 2 - BOARD / 2) + margin + pad * (res % size) - visualData.hint.getRadius(),
                (WIN_Y / 2 - BOARD / 2) + margin + pad * (res / size) - visualData.hint.getRadius()));
            hint = false;
		}


        buttonEvent(&(_visual_.b8), ev, &previewToggle); //toggle la preview rouge
        if (visualData.scoreState) {
            getScore(visualData.map);
        }
    } 
}

// reset le jeu et reviens � l'ecran titre (si vous voyez des comportement bizarre apres lors d'une deuxieme partie ca viendra peut-etre de la)
void Game::resetGame() {
    visualData.bScore = 0;
    visualData.wScore = 0;
    visualData.gameOn = false;
    visualData.gomoku = false;
    visualData.endGame = false;
    visualData.scoreState = false;
    visualData.victoryScreen = false;
	visualData.totalTurn = 0;
    turn = true;
    pass = 0;
    _visual_.b6.setText("pass");
    _visual_.b6.setButtonTexture(_visual_.getT1());
    hint = false;
    lastTurn = false;
    previewToggle = true;
    gomokuOn = false;
    goOn = false;
    memset(visualData.map, '0', 361);
}


// fonction qui v�rifie si la game de gomoku est finie ou si le prochain coup empeche une victoire
void Game::mokuVictory(int x, int y) {
    char p = visualData.map[y * size + x];
    double pad = (BOARD - (MIN_MARGIN * 2)) / (size - 1);
    double margin = (BOARD - (pad * (size - 1))) / 2;

    if (visualData.bScore == 5 || visualData.wScore == 5) { // score � 5 victoire, incontestable
        visualData.endGame = true;
        return;
    }
    if (lastTurn) { // il y a une ligne de 5 mais le coup actuel a pu la briser
        for (int i = 0; i <= 4; i++) { // V�rifie l'int�grit� de la ligne
            if (visualData.map[wLine[0] + ((-wLine[1] * i) + ((-wLine[2] * i) * size))] == '0') {
                lastTurn = false;
                break; // pour setup l'ecran de fin � la fin de la fonction
            }
        }
        if (lastTurn) { // ligne pas bris�e fin de jeu
            visualData.endGame = true;
            visualData.wScore = (turn) ? 999 : 0;
            visualData.bScore = (turn) ? 0 : 999;
            return;
        }
    }
    if (fivePound(x, y, p))
        lastTurn = false;
    else
        return;
    visualData.endGame = true;
    visualData.line[0] = sf::Vertex(sf::Vector2f((WIN_X / 2 - BOARD / 2) + margin + pad * (wLine[0] % size), (WIN_Y / 2 - BOARD / 2) + margin + pad * (wLine[0] / size)),
        sf::Color::Green);
    visualData.line[1] = sf::Vertex(sf::Vector2f((WIN_X / 2 - BOARD / 2) + margin + pad * (wLine[0] % size) + ((pad * 4) * -wLine[1]), (WIN_Y / 2 - BOARD / 2) + margin + pad * (wLine[0] / size) + ((pad * 4) * -wLine[2])),
        sf::Color::Green);
    if ((turn && visualData.bScore < 4) || (!turn && visualData.wScore < 4)) {
        for (int i = 0; i <= 4; i++) {
            if (vulnerable((wLine[0] % size) + (-wLine[1] * i), (wLine[0] / size) + (-wLine[2] * i), p)) {
                lastTurn = true;
                visualData.endGame = false;
                break;
            }
        }
    }
    else if ((turn && visualData.bScore == 4) || (!turn && visualData.wScore == 4))
        for (int i = 0; i < size * size; i++) {
            if (visualData.map[i] == p && vulnerable(i % size, i / size, p)) {
                lastTurn = true;
                visualData.endGame = false;
                break;
            }
        }
    if (visualData.endGame) {
        visualData.bScore = (turn) ? 999 : 0;
        visualData.wScore = (turn) ? 0 : 999;
    }
}

bool Game::vulnerable(int x, int y, char p) {
    char e = (p == '1') ? '2' : '1';
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dx == 0 && dy == 0)
                continue;
            if (x + dx >= 0 && x + dx < size && y + dy >= 0 && y + dy < size && visualData.map[(y + dy) * size + x + dx] == p) {
                if (x + (dx * 2) >= 0 && x + (dx * 2) < size && y + (dy * 2) >= 0 && y + (dy * 2) < size && visualData.map[(y + (dy * 2)) * size + x + (dx * 2)] == e &&
                    x + (dx * -1) >= 0 && x + (dx * -1) < size && y + (dy * -1) >= 0 && y + (dy * -1) < size && visualData.map[(y + (dy * -1)) * size + x + (dx * -1)] == '0') {
                    savingMove = (y + (dy * -1)) * size + x + (dx * -1);
                    return true;
                }
                else if (x + (dx * 2) >= 0 && x + (dx * 2) < size && y + (dy * 2) >= 0 && y + (dy * 2) < size && visualData.map[(y + (dy * 2)) * size + x + (dx * 2)] == '0' &&
                    x + (dx * -1) >= 0 && x + (dx * -1) < size && y + (dy * -1) >= 0 && y + (dy * -1) < size && visualData.map[(y + (dy * -1)) * size + x + (dx * -1)] == e) {
                    savingMove = (y + (dy * 2)) * size + x + (dx * 2);
                    return true;
                }
            }
        }
    }
    return false;
}

// ca check et stock une ligne de 5 en cas de contestation
bool Game::fivePound(int x, int y, char p) {
    int first = (y) * size + (x);
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            int ally = 0;
            if (dy == 0 && dx == 0)
                return false;
            for (int i = 1;
                !(x + dx * i < 0 || x + dx * i >= size || y + dy * i < 0 || y + dy * i >= size)
                && visualData.map[(y + dy * i) * size + (x + dx * i)] == p && ally < 4;
                i++) {
                ally++;
                first = (y + dy * i) * size + (x + dx * i);
            }
            for (int i = 1;
                !(x + -dx * i < 0 || x + -dx * i >= size || y + -dy * i < 0 || y + -dy * i >= size)
                && visualData.map[(y + -dy * i) * size + (x + -dx * i)] == p && ally < 4;
                i++) {
                ally++;
            }
            if (ally >= 4) {
                wLine[0] = first;
                wLine[1] = dx;
                wLine[2] = dy;
                return true;
            }
        }
    }
    return false;
}

//Fonction de prise pour le jeu du Go
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

//detecte si y'a un coup interdit et provoque les prises
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
            if (map[ny * size + nx] == e && mokuTake(dx, dy, pos % size, pos / size, map)) { // Prise de pions
                map[ny * size + nx] = '0';
                map[ny * size + size * dy + nx + dx] = '0';
                visualData.bScore += (turn) ? 1 : 0;
                visualData.wScore += (turn) ? 0 : 1;
                if (visualData.bScore == 5 || visualData.wScore == 5)
                    visualData.endGame = true;
                took = true;
            }
            if (!took && threeLineDetector(dx, dy, pos % size, pos / size, map)) { // en gros ca rentrera la 2 fois pour trigger l'interdiction si ya pas de prise
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

// v�rifie si une prise est possible
bool Game::mokuTake(int dx, int dy, int x, int y, char* m) {
    if ((x + dx * 3 >= 0 && x + dx * 3 < size && y + dy * 3 >= 0 && y + dy * 3< size)
        && m[(y * size + ((dy * size) * 2)) + x + (dx * 2)] != m[y * size + x]
        && m[(y * size + ((dy * size) * 2)) + x + (dx * 2)] != '0'
        && m[(y * size + ((dy * size) * 3)) + x + (dx * 3)] == m[y * size + x]) {
        return (true);
    }
    return(false);
}

// Fonction qui detecte si une position et une direction provoque un threeline
bool Game::threeLineDetector(int dx, int dy, int x, int y, char* map) {
    char    p = map[y * size + x];
    char    e = (p == '1') ? '2' : '1';
    int     hole = 0;
    int     ally = 0;
    bool    safeBreak = false;
    bool    lastHitEmpty = false;
    int     ennemy;

    for (int i = 1; i < 5 && !(x + dx * i < 0 || x + dx * i >= size || y + dy * i < 0 || y + dy * i >= size); i++) {
        lastHitEmpty = false;
        if (map[(y * size + ((dy * size) * i)) + x + (dx * i)] == e) {
            if (map[(y * size + ((dy * size) * (i - 1))) + x + (dx * (i - 1))] == p)
                return false;
            ennemy = i - 1;
            safeBreak = true;
            break;
        }
        else if (map[(y * size + ((dy * size) * i)) + x + (dx * i)] == '0') {
            hole++;
            lastHitEmpty = true;
            if (ally == 2 || hole == 2) {
                safeBreak = true;
                break;
            }
        }
        else if (map[(y * size + ((dy * size) * i)) + x + (dx * i)] == p) {
            if (ally == 2)
                return false;
            ally++;
        }
    }
    dy *= -1;
    dx *= -1;
    if (!safeBreak && !lastHitEmpty)
        return false;
    if (ally == 2)
        return ((y + dy >= 0 && y + dy < size&& x + dx < size&& x + dx >= 0) && map[((y + dy) * size) + x + dx] == '0');
    else if (ally == 1 && dy != 1 && !(dx == 1 && dy == 0)
        && x + (dx * 2) >= 0 && x + (dx * 2) < size
        && y + (dy * 2) >= 0 && y + (dy * 2) < size
        && map[((y + dy) * size) + x + dx] == p
        && map[((y + (dy * 2)) * size) + (x + (dx * 2))] == '0')
            return(true);
    return false;
}

//fonction qui dit si le territoire de la position est sujet � la mort
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