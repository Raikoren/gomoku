#ifndef ALGO_HPP
# define ALGO_HPP

# include <iostream>
# include <string.h>
#include <chrono>
#include <vector>
#include <utility>
#include "env.hpp"

struct AlgoData {
    int     wScore;// une fois a 5 l'algo ne doit pas aller plus loin
    int     bScore;// une fois a 5 l'algo ne doit pas aller plus loin
    char    map[361];// map original
    bool    turn;// true = black || false == white
    int     lastPound;// position du dernier pion pos�
    int     size;
};

class Algo {
	public:
		Algo() { }
        int     ask(AlgoData data); // fonction d'appel
        int     minMax(std::vector<std::string>::iterator position, int alpha, int beta, int depth, bool turn);
        // position = position dans l'historique (0 <-> DEPTH)
        // alpha = meilleur score actuel pour les blancs
        // beta = meilleur score actuel pour les noirs
        // depth = profondeur actuelle
        std::vector<int> setMovesOrder(std::vector<std::string>::iterator i, bool turn); // retourne un vecteur contenent dans l'ordre les coups � tester 
        bool    checkPos(int x, int y, std::string map, bool firstRound, bool turn); // check valibilit� d'un coup et si firstRound == true, si le coup cr�� une ligne ou une prise
		int		heuristique(int currentEval, int newEval);
        bool    threeLine(int dx, int dy, int x, int y, const std::string map, bool turn);
        bool    canTake(int x, int y, std::string map, bool turn);
	private:
        int                         bScore;
        int                         wScore;
        int                         size;
        int                         lastPoundY;
        int                         lastPoundX;
        std::vector<std::string>    historique; //historique de longueur DEPTH
        std::vector<std::string>    movesOrder; //historique de longueur DEPTH
};

#endif