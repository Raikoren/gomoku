#ifndef GAME_HPP
#define GAME_HPP

# include "visual.hpp"
# include <iostream>

class Game/* : public [outils utile plus tard comme l'ia]*/
{
private:
    sf::Sound   click;
    sf::Music   music;
    Visual      _visual_;

public:
    Game(/* args */);
    ~Game();

    void run();
    //void aiPlay();
    //void clear();
};
#endif