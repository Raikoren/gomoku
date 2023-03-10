#include "algo.hpp"


// v�rifie si une prise est possible
bool Algo::mokuTake_algo(int dx, int dy, int x, int y, char* m, int size) {
    if ((x + dx * 3 >= 0 && x + dx * 3 < size && y + dy * 3 >= 0 && y + dy * 3< size)
        && m[(y * size + ((dy * size) * 2)) + x + (dx * 2)] != m[y * size + x]
        && m[(y * size + ((dy * size) * 2)) + x + (dx * 2)] != '0'
        && m[(y * size + ((dy * size) * 3)) + x + (dx * 3)] == m[y * size + x]) {
        return (true);
    }
    return(false);
}

int Algo::min(int minEval, int eval) {
	if (minEval < eval) {
		return minEval;
	}
	else {
		return eval;
	}
}

int Algo::max(int maxEval, int eval) {
	if (maxEval > eval) {
		return maxEval;
	}
	else {
		return eval;
	}
}


// int minimax(int depth, bool maximizingPlayer, int alpha, int beta, int position, int coup);
int Algo::minimax(int depth, bool maximizingPlayer, int alpha, int beta, int position, int coup) {
	if (depth == 0) {
		return ;// static evaluation of position
	}
	if (maximizingPlayer) {
		int maxEval = -1000000;
		for (int i = 0; i < 361; i++) {
			int eval = minimax(depth - 1, false, alpha, beta, position, coup);
			maxEval = max(maxEval, eval);
			alpha = max(alpha, eval);
			if (beta <= alpha) {
				break;
			}
		}
		return maxEval;
	}
	else {
		int minEval = 1000000;
		for (int i = 0; i < 361; i++) {
			int eval = minimax(depth - 1, true, alpha, beta, position, coup);
			minEval = min(minEval, eval);
			beta = min(beta, eval);
			if (beta <= alpha) {
				break;
			}
		}
		return minEval;
	}
}

int Algo::Take_algo(char *map, int mapSize, int player) {
	dprintf(1, "player = %d\n", player);
	int size = 19; // pour le moment
	if (player == 0) {
		// le pion a poser sera 1
		// je veux voir si je peux me poser a coter d'un pion 1 sur la map
		for (int i = 0; i < mapSize; i++) {
			if (map[i] == '2'){
				dprintf(1, "pion 1 a la position %d\n", i);
				// maintenant je veux voir si je peux poser un pion a coter de ce pion 
				// que sa soit  haut bas gauche droite
				if (i - size >= 0) {
					if (map[i - size] == '0') {
						// dprintf(1, "je peux poser a la position haut %d\n", i - size);
						char *map_temp = map;
						map_temp[i - size] = '1';
						bool result = mokuTake_algo(0, -1, (i - size) % size, (i - size) / size, map, size);
						if (result == true) {
							dprintf(1, "je peux poser a la position haut %d\n", i - size);
						}
						map_temp[i - size] = '0';
					}
				}
				if (i + size < mapSize) {
					if (map[i + size] == '0') {
						// dprintf(1, "je peux poser a la position bas %d\n", i + size);
						char *map_temp = map;
						map_temp[i + size] = '1';
						bool result = mokuTake_algo(0, 1, (i + size) % size, (i + size) / size, map, size);
						if (result == true) {
							dprintf(1, "je peux poser a la position bas %d\n", i + size);
						}
						map_temp[i + size] = '0';
					}
				}
				if (i - 1 >= 0) {
					if (map[i - 1] == '0') {
						// dprintf(1, "je peux poser a la position gauche %d\n", i - 1);
						char *map_temp = map;
						map_temp[i - 1] = '1';
						bool result = mokuTake_algo(1, 0, (i - 1) % size, (i - 1) / size, map, size);
						if (result == true) {
							dprintf(1, "je peux poser a la position gauche %d\n", i - 1);
						}
						map_temp[i - 1] = '0';
					}
				}
				if (i + 1 < mapSize) {
					if (map[i + 1] == '0') {
						// dprintf(1, "je peux poser a la position droite %d\n", i + 1);
						char *map_temp = map;
						map_temp[i + 1] = '1';
						bool result = mokuTake_algo(-1, 0,  (i + 1) % size,  (i + 1) / size, map, size);
						if (result == true) {
							dprintf(1, "je peux poser a la position droite %d\n", i + 1);
						}
						map_temp[i + 1] = '0';
					}
				}
			}
		}
	}
	else {
		// le pion a poser sera 2
		// je veux voir si je peux me poser a coter d'un pion 2 sur la map
		for (int i = 0; i < mapSize; i++) {
			if (map[i] == '1')
				dprintf(1, "pion 2 a la position %d\n", i);
		}
		
	}
	dprintf(1, "test\n");
	return 0;
}


int evaluerPosition_line(char *grille, char joueur) {
    int score = 0;
	int N = 19; // taille de la grille

    // Evaluer les lignes horizontales
    for (int i = 0; i < N; i++) {
        for (int j = 0; j <= N - 3; j++) {
			if (*(grille + i*N + j) == joueur && *(grille + i*N + j+1) == joueur) {
                score += 3;
            }
            else if (*(grille + i*N + j) == joueur && *(grille + i*N + j+1) == joueur && *(grille + i*N + j+2) == joueur) {
                score += 10;
            } else if (j <= N - 4 && *(grille + i*N + j) == joueur && *(grille + i*N + j+1) == joueur && *(grille + i*N + j+2) == joueur && *(grille + i*N + j+3) == joueur) {
                score += 50;
            } else if (j <= N - 5 && *(grille + i*N + j) == joueur && *(grille + i*N + j+1) == joueur && *(grille + i*N + j+2) == joueur && *(grille + i*N + j+3) == joueur && *(grille + i*N + j+4) == joueur) {
                return INT_MAX; // joueur a gagné
            }
        }
    }
    // Evaluer les lignes verticales
    for (int i = 0; i <= N - 3; i++) {
        for (int j = 0; j < N; j++) {
			 if (*(grille + i*N + j) == joueur && *(grille + (i+1)*N + j) == joueur) {
                score += 3;
            }
            else if (*(grille + i*N + j) == joueur && *(grille + (i+1)*N + j) == joueur && *(grille + (i+2)*N + j) == joueur) {
                score += 10;
            } else if (i <= N - 4 && *(grille + i*N + j) == joueur && *(grille + (i+1)*N + j) == joueur && *(grille + (i+2)*N + j) == joueur && *(grille + (i+3)*N + j) == joueur) {
                score += 50;
            } else if (i <= N - 5 && *(grille + i*N + j) == joueur && *(grille + (i+1)*N + j) == joueur && *(grille + (i+2)*N + j) == joueur && *(grille + (i+3)*N + j) == joueur && *(grille + (i+4)*N + j) == joueur) {
                return INT_MAX; // joueur a gagné
            }
        }
    }
	    // Evaluer les diagonales de haut en bas
    for (int i = 0; i <= N - 3; i++) {
        for (int j = 0; j <= N - 3; j++) {
            int index = i * N + j;
			if (grille[index] == joueur && grille[index + N + 1] == joueur) {
                score += 3;
            }
            else if (grille[index] == joueur && grille[index + N + 1] == joueur && grille[index + 2 * N + 2] == joueur) {
                score += 10;
            } else if (i <= N - 4 && j <= N - 4 && grille[index] == joueur && grille[index + N + 1] == joueur && grille[index + 2 * N + 2] == joueur && grille[index + 3 * N + 3] == joueur) {
                score += 50;
            } else if (i <= N - 5 && j <= N - 5 && grille[index] == joueur && grille[index + N + 1] == joueur && grille[index + 2 * N + 2] == joueur && grille[index + 3 * N + 3] == joueur && grille[index + 4 * N + 4] == joueur) {
                return INT_MAX; // joueur a gagné
            }
        }
    }
    // Evaluer les diagonales de bas en haut
    for (int i = N - 1; i >= 4; i--) {
        for (int j = 0; j <= N-4; j++) {
            int index = i * N + j;
			if (grille[index] == joueur && grille[index - N + 1] == joueur) {
                score += 3;
            }
            else if (grille[index] == joueur && grille[index - N + 1] == joueur && grille[index - 2 * N + 2] == joueur) {
                score += 10;
            } else if (i >= 3 && j <= N-4 && grille[index] == joueur && grille[index - N + 1] == joueur && grille[index - 2 * N + 2] == joueur && grille[index - 3 * N + 3] == joueur) {
                score += 50;
            } else if (i >= 4 && j <= N-5 && grille[index] == joueur && grille[index - N + 1] == joueur && grille[index - 2 * N + 2] == joueur && grille[index - 3 * N + 3] == joueur && grille[index - 4 * N + 4] == joueur) {
                return INT_MAX; // joueur a gagné
            }
        }
    }
    return score;
}

// int evaluer_score(char *grille, char joueur) {

// }




int Algo::test_main(char *map, int mapSize, int player, int black_score, int white_score) {
	// je veux qu'il calcule le score de la map en fonction du player

	dprintf(1, "back_score = %d\nwhite_score = %d\n", black_score, white_score);
	int score = 0;
	// black 2
	// white 1

	char temp_player;
	char other_player;

	if (player == 1){
		temp_player = '2';
		other_player = '1';
	}
	else {
		temp_player = '1';
		other_player = '2';
	}
	if (black_score == INT_MAX || white_score == INT_MAX)
		return INT_MAX;

	score = evaluerPosition_line(map, temp_player);
	score -= evaluerPosition_line(map, other_player);

	return score;

	dprintf(1, "player = %c\nscore = %d\n", temp_player, score);
}