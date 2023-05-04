#ifndef ALGO_HPP
# define ALGO_HPP

# include <iostream>
# include <string.h>
#include <chrono>
#include <vector>
#include <utility>
#include <tgmath.h>
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
        int     minMax(const std::string& position, int alpha, int beta, int depth, bool turn);
        // position = position dans l'historique (0 <-> DEPTH)
        // alpha = meilleur score actuel pour les blancs
        // beta = meilleur score actuel pour les noirs
        // depth = profondeur actuelle
        std::vector<int> setMovesOrder(const std::string& i, bool turn); // retourne un vecteur contenent dans l'ordre les coups � tester 
		std::vector<std::pair<int, int>> getWindowBounds(const std::string& map);
        bool    checkPos(int x, int y, std::string map, bool firstRound, bool turn); // check valibilit� d'un coup et si firstRound == true, si le coup cr�� une ligne ou une prise
		int		heuristique(const std::string& map, bool turn);
        bool    threeLine(int dx, int dy, int x, int y, const std::string map, bool turn);
		bool	fourLine(int dx, int dy, int x, int y, const std::string map, bool turn);
		int		calculateScoreRow(const std::string& map, char player);
        bool    canTake(int x, int y, std::string map, bool turn);
		bool	FiveInRow(const std::string& map, bool turn, char player);
	private:

		int							optimalMove;
        int                         bScore;
        int                         wScore;
        int                         size;
        int                         lastPoundY;
        int                         lastPoundX;
		bool						player_dark;
        std::vector<std::string>    historique; //historique de longueur DEPTH
        std::vector<std::string>    movesOrder; //historique de longueur DEPTH
};

#endif