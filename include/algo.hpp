#ifndef ALGO_HPP
# define ALGO_HPP

# include <iostream>
# include <string.h>
#include <chrono>
#include <vector>
#include <utility>
// # include "game.hpp"

class Algo {
	public:
		Algo() { }

		std::vector<std::pair<int, int>> get_windows(char *map, int mapSize, int margin) ;

		int coup(char *map, bool joueur);

		int test_main(char *map, int mapSize, int player, int black_score, int white_score);

		int minimax_v2(char *map, int depth, bool EstMax, int alpha, int beta);
		int evaluerPosition_line(char *grille, char joueur);
		int evaluerLigne(int ligne_score, char joueur);

		int mtd_f(char *map, int depth, int f);

	private:
		// Game	game;
		// char map[361];
		// int mapSize;

};

#endif