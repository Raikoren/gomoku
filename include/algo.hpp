#ifndef ALGO_HPP
# define ALGO_HPP

# include <iostream>
# include <string.h>
#include <chrono>
#include <vector>
#include <unordered_map>
#include <utility>
// # include "game.hpp"

// TranspositionTable class
class TranspositionTable {
public:
    struct TableEntry {
        int depth;
        int value;
        int flag;
    };

    enum EntryFlag {
        EXACT, LOWERBOUND, UPPERBOUND
    };

    void store(int hash_key, int value, int depth, int flag) {
        TableEntry entry{depth, value, flag};
        table[hash_key] = entry;
    }

    TableEntry* get(int hash_key) {
        auto it = table.find(hash_key);
        if (it != table.end()) {
            return &it->second;
        }
        return nullptr;
    }

private:
    std::unordered_map<int, TableEntry> table;
};

class Algo {
	public:
		Algo() { }
		TranspositionTable transposition_table;


		std::vector<std::pair<int, int>> get_windows(char *map, int mapSize, int margin) ;

		int coup(char *map, bool joueur);

		int test_main(char *map, int mapSize, bool player, int black_score, int white_score);

		int minimax_v2(char *map, int depth, bool EstMax, int alpha, int beta, char joueur, char adversaire);
		int evaluerPosition_line(char *grille, char joueur);
		int evaluerLigne(int ligne_score, char joueur);

		int mtd_f(char *map, int depth, int f, char joueur, char adversaire);

		int count_tokens_in_direction(char *grille, char joueur, int i, int j, int dx, int dy);
		int evaluerPosition_line_joueur1(char *grille);
		int evaluerPosition_line_joueur2(char *grille);
		std::vector<std::pair<int, int>> get_adjacent_positions(char *map, int mapSize);

	private:
		// Game	game;
		// char map[361];
		// int mapSize;

};

#endif