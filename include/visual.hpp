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
    bool            gameOn = false;
    bool            gomoku = false;
    bool            endGame = false;
    bool            scoreState = false;
    bool            victoryScreen = false;
    int             size;
    char            map[361];
    char            result[361];
    sf::CircleShape preview;
    bool            previewEnable = false;
    int             bScore = 0;
    int             wScore = 0;
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
    Button b7;
    Button b8;
    Button b9;
    sf::CircleShape bPound;
    sf::CircleShape wPound;
    sf::CircleShape territory;


protected:
    sf::RenderWindow _window_;
    int size = BIG_BOARD;
    sf::Texture titleBg;
    sf::Font f;

};
#endif