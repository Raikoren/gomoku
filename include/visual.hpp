#ifndef VISUAL_HPP
# define VISUAL_HPP

# include <iostream>
# include <vector>
# include <SFML/Graphics.hpp>
# include <SFML/Audio.hpp>
# include "env.hpp"
# include "button.hpp"

struct visual_data
{
    bool            gameOn;
    bool            victoryScreen;
    int             size;
    char            map[361];
    sf::CircleShape preview;
    bool            previewEnable = false;
};

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

    void draw(visual_data visualData);
    void drawBoard(visual_data v);
    void drawTitleScreen();
    void drawButtons();
    void drawPounds(visual_data v, double margin, double pad);

    Button b1;
    Button b2;
    Button b3;
    Button b4;
    Button b5;
    Button b6;
    sf::CircleShape bPound;
    sf::CircleShape wPound;


protected:
    sf::RenderWindow _window_;
    int size = BIG_BOARD;
    sf::Texture titleBg;
    sf::Font f;

};
#endif