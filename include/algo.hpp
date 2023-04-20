#ifndef ALGO_HPP
# define ALGO_HPP

# include <iostream>
# include <string.h>
#include <chrono>
#include <vector>
#include <utility>
// # include "game.hpp"

struct algoData {
    int     wScore;// une fois a 5 l'algo ne doit pas aller plus loin
    int     bScore;// une fois a 5 l'algo ne doit pas aller plus loin
    char    map[362];// map original
    bool    turn;// true = blanc || false == noir
};

class Algo {
	public:
		Algo() { }
        int     algo(algoData data); // fonction d'appel
        void    minMax(int position, float alpha, float beta, int depth, bool turn);
        // position = position dans l'historique (0 <-> DEPTH)
        // alpha = meilleur score actuel pour les blancs
        // beta = meilleur score actuel pour les noirs
        // depth = profondeur actuelle
        bool    newPosition(int child, int position);
	private:
        int     bScore;
        int     wScore;
        // vector historique de taille DEPTH
};

#endif