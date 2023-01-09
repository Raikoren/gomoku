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
    int         size;
    bool        titleScreen;
    visual_data visualData;
    bool        turn = false;

public:
    Game();
    ~Game();

    void run();
    void settingUp(sf::Event ev);
    void buttonEvent(Button* b, sf::Event ev, bool* modified, bool modifier);
    void buttonEvent(Button* b, sf::Event ev, int* modified, int modifier);
    void gaming(sf::Event ev);
    bool targetingBoard(sf::Event, sf::RenderWindow& w, sf::Vector2f m, double p);

    int getSize() {
        return(size);
    }

    bool getTurn() {
        return(turn);
    }
};
#endif