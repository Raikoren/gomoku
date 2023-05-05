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
    bool                gameOn = false;
    bool                gomoku = false;
    bool                endGame = false;
    bool                scoreState = false;
    bool                victoryScreen = false;
    int                 size;
    char                map[361];
    char                result[361];
    sf::CircleShape     preview;
    sf::CircleShape     hint;
    bool                previewEnable = false;
    int                 bScore = 0;
    int                 wScore = 0;
    sf::Vertex          line[2];
    bool                hintOn = false;
    int                 totalTurn = 0;
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

    void setGoban(int i) {
        if (i == 9)
            if (!goban.loadFromFile("ressources/goban9.png"))
		        std::cout << "Error while loading button texture" << std::endl;
        if (i == 13)
            if (!goban.loadFromFile("ressources/goban13.png"))
                std::cout << "Error while loading button texture" << std::endl;
        if (i == 19)
            if (!goban.loadFromFile("ressources/goban19.png"))
                std::cout << "Error while loading button texture" << std::endl;
    }
    sf::Texture* getT1() {
        return(&sbTextureI);
    }
    sf::Texture* getT2() {
        return(&sbTextureM);
    }
    sf::Texture* getT3() {
        return(&sbTextureC);
    }
    sf::Texture* getT4() {
        return(&goTextI);
    }
    sf::Texture* getT5() {
        return(&goTextM);
    }
    sf::Texture* getT6() {
        return(&mokuTextI);
    }
    sf::Texture* getT7() {
        return(&mokuTextM);
    }
    sf::Text* getGameState() {
        return &gameState;
    }

    Button b1;// set map size > 9
    Button b2;// set map size > 13
    Button b3;// set map size > 19
    Button b4;// start gomoku mode
    Button b5;// start go mode
    Button b6;// go mode: pass
    Button b7;// gomoku mode: algo's proposition
    Button b8;// toggle red preview
    Button b9;// go mode : score's preview
    Button bReturn; // go back to title screen
    Button bVsAi; // toggle VsAI
    sf::CircleShape bPound;
    sf::CircleShape wPound;
    sf::CircleShape territory;


protected:
    sf::RenderWindow _window_;
    int size = BIG_BOARD;
    sf::Texture titleBg;
    sf::Texture sbTextureI;
    sf::Texture sbTextureM;
    sf::Texture sbTextureC;
    sf::Texture goTextI;
    sf::Texture goTextM;
    sf::Texture mokuTextI;
    sf::Texture mokuTextM;
    sf::Texture textSB;
    sf::Texture textSW;
    sf::Texture goban;
    sf::Texture egalite;
    sf::Texture bWin;
    sf::Texture wWin;
    sf::Font f;
    sf::Text gameState;

};
#endif