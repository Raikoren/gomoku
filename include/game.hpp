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
    bool        mainMenu = true;
    int         size;

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