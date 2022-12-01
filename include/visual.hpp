#ifndef VISUAL_HPP
# define VISUAL_HPP

# include <iostream>
# include <vector>
# include <SFML/Graphics.hpp>
# include <SFML/Audio.hpp>
# include "env.hpp"

class Visual
{
public:
    Visual();
    ~Visual() {
        _window_.clear();
    }
    sf::RenderWindow* getWin() {
        return &_window_;
    }

    void draw();
    void grid();
    void setCase();


protected:
    sf::RenderWindow _window_;
    int size = BIG_BOARD;
};

#endif