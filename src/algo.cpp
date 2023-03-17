#include "algo.hpp"
#include <chrono>


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
int Algo::minimax(int depth, bool maximizingPlayer, int alpha, int beta, char *map) {
	if (depth == 0) {
		// static evaluation of position
		// dprintf(1, "depth = 0\n map = %s\n", map);
		// printf("%d\n", test_main(map, 19, maximizingPlayer, 0, 0));
		dprintf(1, "map = %s\n", map);
		return test_main(map, 19, maximizingPlayer, 0, 0);
	}
	// dprintf(1,"depth = %d\n", depth);
	// dprintf(1,"maximizingPlayer = %d\n", maximizingPlayer);
	if (maximizingPlayer) {
		int maxEval = -1000000;
		for (int i = 0; i < 361; i++) {
			if (map[i] != '0') {
				continue;
			}
			char newMap[361];
			memcpy(newMap, map, 361);
			newMap[i] = '1';
			// dprintf(1, "newMap = %s\n", newMap);
			int eval = minimax(depth - 1, false, alpha, beta, newMap);
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
			if (map[i] != '0') {
				continue;
			}
			char newMap[361];
			memcpy(newMap, map, 361);
			newMap[i] = '2';
			int eval = minimax(depth - 1, true, alpha, beta, newMap);
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


int Algo::evaluerPosition_line(char *grille, char joueur) {
    int score = 0;
    int N = 19; // taille de la grille

    // Directions : horizontale, verticale, diagonale droite, diagonale gauche
    int direction[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (grille[i * N + j] == joueur) {
                for (int d = 0; d < 4; ++d) {
                    int count = 1;

                    for (int k = 1; k < 5; ++k) {
                        int x = i + k * direction[d][0];
                        int y = j + k * direction[d][1];
                        if (0 <= x && x < N && 0 <= y && y < N) {
                            if (grille[x * N + y] == joueur) {
                                count += 1;
                            } else {
                                break;
                            }
                        } else {
                            break;
                        }
                    }

                    score += evaluerLigne(count, joueur);
                }
            }
        }
    }

    return score;
}

// Fonction qui évalue une ligne de jetons
int Algo::evaluerLigne(int ligne_score, char joueur) {
    switch(ligne_score) {
        case 1:
            return 1;
        case 2:
            return 10;
        case 3:
            return 100;
        case 4:
            return 1000;
        case 5:
            return 10000;
        default:
            return 0;
    }
}




int Algo::test_main(char *map, int mapSize, int player, int black_score, int white_score) {
	// je veux qu'il calcule le score de la map en fonction du player

	// dprintf(1, "back_score = %d\nwhite_score = %d\n", black_score, white_score);
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
	if (score == INT_MAX)
		return INT_MAX;
	score -= evaluerPosition_line(map, other_player);
	if (score == INT_MAX)
		return INT_MAX;

	return score;
}

int Algo::minimax_v2(char *map, int depth, bool EstMax, int alpha, int beta) {
    // je veux une variable globale qui compte le nombre de fois que la fonction est appelée
    static int call_count = 0;
    call_count++;

    if (test_main(map, 19, true, 0, 0) == INT_MAX) {
        return INT_MAX;
    }

    else if (test_main(map, 19, false, 0, 0) == INT_MAX) {
        return INT_MIN;
    }

    if (depth == 0) {
        return test_main(map, 19, true, 0, 0);
    }

    int score;
    if (EstMax) {
        int meilleur_score = INT_MIN;
        for (int i = 0; i < 19; i++) {
            for (int j = 0; j < 19; j++) {
                if (map[i * 19 + j] == '0') {
                    map[i * 19 + j] = '2'; // 2 pour le moment
                    score = minimax_v2(map, depth - 1, false, alpha, beta);
                    map[i * 19 + j] = '0';
                    meilleur_score = std::max(meilleur_score, score);

                    alpha = std::max(alpha, meilleur_score);
                    if (beta <= alpha) { // Élagage bêta
                        return meilleur_score;
                    }
                }
            }
        }
        return meilleur_score;
    }
    else {
        int meilleur_score = INT_MAX;
        for (int i = 0; i < 19; i++) {
            for (int j = 0; j < 19; j++) {
                if (map[i * 19 + j] == '0') {
                    map[i * 19 + j] = '1'; // 1 pour le moment
                    score = minimax_v2(map, depth - 1, true, alpha, beta);
                    map[i * 19 + j] = '0';
                    meilleur_score = std::min(meilleur_score, score);

                    beta = std::min(beta, meilleur_score);
                    if (beta <= alpha) { // Élagage alpha
                        return meilleur_score;
                    }
                }
            }
        }
        return meilleur_score;
    }
}


int Algo::coup(char *map, bool joueur) {
    int meilleur_coup;
    int mapSize = 19;
    int score = 0;
	char pion;
	dprintf(1, "joueur = %d", joueur);

	if (joueur == true)
		pion = '2';
	else 
		pion = '1';

	int meilleur_score = pion == '1' ? INT_MAX : INT_MIN;

    for (int i = 0; i < mapSize; i++) {
        for (int j = 0; j < mapSize; j++) {
            if (map[i * mapSize + j] == '0') {
                map[i * mapSize + j] = pion;
				// auto start = std::chrono::high_resolution_clock::now();
                if (pion == '1') {
                    score = minimax_v2(map, 2, true, INT_MIN, INT_MAX);
                } else {
                    score = minimax_v2(map, 2, false, INT_MIN, INT_MAX);
                }

				// // Enregistrez le temps de fin juste après l'appel à minimax_v2
                // auto end = std::chrono::high_resolution_clock::now();

                // // Calculez et affichez la durée en millisecondes
                // std::chrono::duration<double, std::milli> elapsed = end - start;
                // std::cout << "Temps écoulé pour minimax_v2 : " << elapsed.count() << " ms" << std::endl;

                map[i * mapSize + j] = '0';

                if ((pion == '1' && score < meilleur_score) || (pion == '2' && score > meilleur_score)) {
                    meilleur_score = score;
                    meilleur_coup = (i * mapSize + j);
                }
            }
        }
    }
    map[meilleur_coup] = pion;
    return meilleur_coup;
}