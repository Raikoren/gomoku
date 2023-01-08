#ifndef GAME_HPP
#define GAME_HPP

# include "visual.hpp"
# include "board.hpp"
# include <iostream>

class Game: public Visual
{
private:
    sf::Sound   click;
    sf::Music   music;
    Visual      _visual_;
    int         size;
    bool        titleScreen;

public:
    Game();
    ~Game();

    void run();
    void settingUp(sf::Event ev);

    int getSize() {
        return(size);
    }
};
#endif