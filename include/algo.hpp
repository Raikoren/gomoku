#ifndef ALGO_HPP
# define ALGO_HPP

# include <iostream>
# include <string.h>
// # include "game.hpp"

class Algo {
	public:
		Algo() { }

		// minimax contient lq position le coup et le maximizingPlayer
		int minimax(int depth, bool maximizingPlayer, int alpha, int beta, int position, int coup);
		int min(int minEval, int eval);
		int max(int maxEval, int eval);

		int Take_algo(char *map, int mapSize, int player);

		bool mokuTake_algo(int dx, int dy, int x, int y, char* m, int size);

		int test_main(char *map, int mapSize, int player, int black_score, int white_score);

	private:
		// Game	game;
		// char map[361];
		// int mapSize;

};

#endif