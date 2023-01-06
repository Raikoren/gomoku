#ifndef VISUAL_HPP
# define VISUAL_HPP

# include <iostream>
# include <vector>
# include <SFML/Graphics.hpp>
# include <SFML/Audio.hpp>
# include "env.hpp"
# include "button.hpp"

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
    void drawBoard();
    void drawTitleScreen();
    void setCase();
    void drawButtons();
    Button b1;
    Button b2;
    Button b3;
    //Button b4;


protected:
    sf::RenderWindow _window_;
    int size = BIG_BOARD;
    sf::Texture titleBg;
    sf::Font f;

};
#endif