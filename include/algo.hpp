#ifndef ALGO_HPP
# define ALGO_HPP

# include <iostream>
# include <string.h>
// # include "game.hpp"

class Algo {
	public:
		Algo() { }

		// minimax contient lq position le coup et le maximizingPlayer
		int minimax(int depth, bool maximizingPlayer, int alpha, int beta, char *map);
		int min(int minEval, int eval);
		int max(int maxEval, int eval);

		int coup(char *map, bool joueur);

		int Take_algo(char *map, int mapSize, int player);

		bool mokuTake_algo(int dx, int dy, int x, int y, char* m, int size);

		int test_main(char *map, int mapSize, int player, int black_score, int white_score);

		int minimax_v2(char *map, int depth, bool EstMax, int alpha, int beta);
		int evaluerPosition_line(char *grille, char joueur);
		int evaluerLigne(int ligne_score, char joueur);

	private:
		// Game	game;
		// char map[361];
		// int mapSize;

};

#endif