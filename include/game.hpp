#ifndef GAME_HPP
# define GAME_HPP

# include "visual.hpp"
# include <iostream>
# include <string.h>

class Game
{
private:
    sf::Sound   click;
    sf::Music   music;
    Visual      _visual_;
    int         size = 9;
    bool        gomokuOn = false;
    bool        goOn = false;
    char        territory[361];
    visual_data visualData;
    bool        turn = true;
    int         pass = 0;
    bool        hint = false;
    bool        lastTurn = false;
    bool        previewToggle = true;
    int         moku[3];
    int         wLine[10];

public:
    Game();
    ~Game();

    void run();
    void settingUp(sf::Event ev);
    void buttonEvent(Button* b, sf::Event ev, bool* modified);
    void buttonEvent(Button* b, sf::Event ev, int* modified, int modifier);
    void gaming(sf::Event ev);
    bool targetingBoard(sf::Event, sf::Vector2f m, double p);
    bool surronded(int pos, char* map);
    bool doubleThreeDetector(int pos, char* map, char p);
    void taking(int pos, char* map);
    void getScore(char* s);
    void mokuVictory(int x, int y);
    bool mokuTake(int dx, int dy, int x, int y, char* m);
    bool threeLineDetector(int dx, int dy, int x, int y, char* map);
    bool fivePound(int dx, int dy, int x, int y, char p);
    bool vulnerable(int x, int y, char p);

    int getSize() {
        return(size);
    }

    bool getTurn() {
        return(turn);
    }
};
#endif