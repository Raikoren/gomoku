#include "algo.hpp"

// map size
#define N 9


std::vector<std::pair<int, int>> Algo::get_windows(char *map, int mapSize, int margin) {
    int x_min = mapSize, x_max = 0, y_min = mapSize, y_max = 0;

    for (int i = 0; i < mapSize; i++) {
        for (int j = 0; j < mapSize; j++) {
            if (map[i * mapSize + j] != '0') {
                x_min = std::min(x_min, i);
                x_max = std::max(x_max, i);
                y_min = std::min(y_min, j);
                y_max = std::max(y_max, j);
            }
        }
    }

    x_min = std::max(x_min - margin, 0);
    x_max = std::min(x_max + margin, mapSize - 1);
    y_min = std::max(y_min - margin, 0);
    y_max = std::min(y_max + margin, mapSize - 1);

    return { {x_min, y_min}, {x_max, y_max} };
}


int Algo::evaluerPosition_line(char *grille, char joueur) {
    int score = 0;
    char adversaire = (joueur == '1') ? '2' : '1';
    int captures = 0;

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
					// Vérifier si l'adversaire peut être bloqué dans cette direction
					if (count <= 2) {
						for (int k = 1; k < 5; ++k) {
							int x = i + k * direction[d][0];
							int y = j + k * direction[d][1];
							if (0 <= x && x < N && 0 <= y && y < N) {
								if (grille[x * N + y] == adversaire) {
									bool blocked = true;
									for (int m = k + 1; m < 5; ++m) {
										int x_block = i + m * direction[d][0];
										int y_block = j + m * direction[d][1];
										if (0 <= x_block && x_block < N && 0 <= y_block && y_block < N) {
											if (grille[x_block * N + y_block] == joueur) {
												blocked = false;
												break;
											}
										} else {
											break;
										}
									}
									if (blocked) {
										score -= 10; // ajouter un score négatif pour bloquer l'adversaire
									}
									break;
								}
							} else {
								break;
							}
						}
					}
					if (count == 5)
						return INT_MAX;
                    score += evaluerLigne(count, joueur);
                }
            } 
			else if (grille[i * N + j] == adversaire) {
                for (int d = 0; d < 4; ++d) {
                    int count = 0;
                    bool is_valid = false;
                    int x1 = i + direction[d][0];
                    int y1 = j + direction[d][1];
                    int x2 = i - direction[d][0];
                    int y2 = j - direction[d][1];
                    if (0 <= x1 && x1 < N && 0 <= y1 && y1 < N &&
                        0 <= x2 && x2 < N && 0 <= y2 && y2 < N) {
                        if (grille[x1 * N + y1] == adversaire &&
                            grille[x2 * N + y2] == joueur) {
                            for (int k = 3; k <= 4; ++k) {
                                int x = i + k * direction[d][0];
                                int y = j + k * direction[d][1];
                                if (0 <= x && x < N && 0 <= y && y < N) {
                                    if (grille[x * N + y] == '0') {
                                        is_valid = true;
                                        captures += 1;
                                        break;
                                    }
                                } else {
                                    break;
                                }
                            }
                        }
                    }
                    if (is_valid) {
                        break;
                    }
                }
            }
        }
    }
    score += captures * 2000; // Ajouter les points de capture
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
            return 100000;
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
	if (score == INT_MAX){
		return INT_MAX;
	}
	score -= evaluerPosition_line(map, other_player);
	if (score == INT_MAX) {
		return INT_MAX;
	}

	return score;
}

int Algo::mtd_f(char *map, int depth, int f) {
    int g = f;
    int beta;
    int upperBound = INT_MAX;
    int lowerBound = INT_MIN;

    while (lowerBound < upperBound) {
        if (g == lowerBound) {
            beta = g + 1;
        } else {
            beta = g;
        }

        g = minimax_v2(map, depth, true, beta - 1, beta);

        if (g < beta) {
            upperBound = g;
        } else {
            lowerBound = g;
        }
    }

    return g;
}


int Algo::minimax_v2(char *map, int depth, bool EstMax, int alpha, int beta) {
    // je veux une variable globale qui compte le nombre de fois que la fonction est appelée
    static int call_count = 0;
    call_count++;

    if (test_main(map, N, true, 0, 0) == INT_MAX) {
        return INT_MAX;
    }

    else if (test_main(map, N, false, 0, 0) == INT_MAX) {
        return INT_MIN;
    }

    if (depth == 0) {
        return test_main(map, N, true, 0, 0);
    }

    int score;
    if (EstMax) {
        int meilleur_score = INT_MIN;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (map[i * N + j] == '0') {
                    map[i * N + j] = '2'; // 2 pour le moment
                    score = minimax_v2(map, depth - 1, false, alpha, beta);
                    map[i * N + j] = '0';
					if (score == INT_MAX) {
						return INT_MAX;
					}
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
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (map[i * N + j] == '0') {
                    map[i * N + j] = '1'; // 1 pour le moment
                    score = minimax_v2(map, depth - 1, true, alpha, beta);
                    map[i * N + j] = '0';
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
	// static coup_count = 0;

	static int call_count = 0;
    int meilleur_coup;
    int mapSize = N;
    int score = 0;
    char pion;
    dprintf(1, "joueur = %d\n", joueur);

    if (joueur == true)
        pion = '2';
    else 
        pion = '1';

    int meilleur_score = pion == '1' ? INT_MAX : INT_MIN;

    // Obtenez les fenêtres rectangulaires
    int margin = 2;
    std::vector<std::pair<int, int>> windows = get_windows(map, mapSize, margin);
    int x_min = windows[0].first;
    int y_min = windows[0].second;
    int x_max = windows[1].first;
    int y_max = windows[1].second;
    // Si x_min et y_min sont proches et que x_max et y_max sont proches
    if (call_count == 0) {
		dprintf(1, "lqqqq\n");
		dprintf(1, "x_min = %d, y_min = %d, x_max = %d, y_max = %d \n", x_min, y_min, x_max, y_max);
        // Se concentrer sur un carré de 5x5 au milieu de la carte
        int mid_x = mapSize / 2;
        int mid_y = mapSize / 2;
        x_min = std::max(mid_x - 2, 0);
        y_min = std::max(mid_y - 2, 0);
        x_max = std::min(mid_x + 2, mapSize - 1);
        y_max = std::min(mid_y + 2, mapSize - 1);
    }
	call_count++;

    for (int i = x_min; i <= x_max; i++) {
        for (int j = y_min; j <= y_max; j++) {
            if (map[i * mapSize + j] == '0') {
                map[i * mapSize + j] = pion;
                if (pion == '1') {
                    score = mtd_f(map, 2, 0);
                }
				else {
                    score = mtd_f(map, 2, 0);
                }
				if (pion == '1' && score == INT_MAX) {
					return meilleur_coup;
				}
				if (pion == '2' && score == INT_MIN) {
					return meilleur_coup;
				}

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
