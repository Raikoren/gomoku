#include "algo.hpp"

// map size
#define N 9
#define SCORE_THRESHOLD 500

std::vector<std::pair<int, int>> Algo::get_adjacent_positions(char *map, int mapSize) {
    std::vector<std::pair<int, int>> adjacent_positions;

    for (int i = 0; i < mapSize; i++) {
        for (int j = 0; j < mapSize; j++) {
            if (map[i * mapSize + j] != '0') {
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        if (dx == 0 && dy == 0) continue;
                        int x = i + dx;
                        int y = j + dy;
                        if (x >= 0 && x < mapSize && y >= 0 && y < mapSize && map[x * mapSize + y] == '0') {
                            adjacent_positions.push_back({x, y});
                        }
                    }
                }
            }
        }
    }

    // Supprimer les doublons
    std::sort(adjacent_positions.begin(), adjacent_positions.end());
    adjacent_positions.erase(std::unique(adjacent_positions.begin(), adjacent_positions.end()), adjacent_positions.end());

    return adjacent_positions;
}


int Algo::evaluerPosition_line_joueur1(char *grille) {
    return evaluerPosition_line(grille, '1');
}

int Algo::evaluerPosition_line_joueur2(char *grille) {
    return evaluerPosition_line(grille, '2');
}


int Algo::count_tokens_in_direction(char *grille, char joueur, int i, int j, int dx, int dy) {
    int count = 0;
    int x = i, y = j;

    while (0 <= x && x < N && 0 <= y && y < N && grille[x * N + y] == joueur) {
        count++;
        x += dx;
        y += dy;
    }

    return count;
}

int Algo::evaluerPosition_line(char *grille, char joueur) {
    int score = 0;
    char adversaire = (joueur == '1') ? '2' : '1';

    // Directions : horizontale, verticale, diagonale droite, diagonale gauche
    int direction[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};

    // Scores pour différentes longueurs de lignes
    int ligne_scores[6] = {0, 1, 10, 100, 1000, 100000};

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (grille[i * N + j] == joueur || grille[i * N + j] == adversaire) {
                for (int d = 0; d < 4; ++d) {
                    int count = count_tokens_in_direction(grille, grille[i * N + j], i, j, direction[d][0], direction[d][1]);

                    // Ajouter le score correspondant
                    score += (grille[i * N + j] == joueur ? 1 : -1) * ligne_scores[count];
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
            return 100000;
        default:
            return 0;
    }
}




int Algo::test_main(char *map, int mapSize, bool player, int black_score, int white_score) {
	// je veux qu'il calcule le score de la map en fonction du player

	// dprintf(1, "back_score = %d\nwhite_score = %d\n", black_score, white_score);
	int score = 0;
	// black 2
	// white 1

	char temp_player;
	char other_player;

	if (player == true){
		temp_player = '2';
		other_player = '1';
	}
	else {
		temp_player = '1';
		other_player = '2';
	}
	if (black_score == INT_MAX || white_score == INT_MAX)
        return INT_MAX;

    score = player ? evaluerPosition_line_joueur2(map) : evaluerPosition_line_joueur1(map);

    if (score == INT_MAX){
        return INT_MAX;
    }
    score -= player ? evaluerPosition_line_joueur1(map) : evaluerPosition_line_joueur2(map);
    if (evaluerPosition_line(map, other_player) == INT_MAX) {
        score = INT_MIN;
        return INT_MIN;
    }


	return score;
}

int Algo::mtd_f(char *map, int depth, int f, char joueur, char adversaire) {
    int g = f;
    int beta;
    int upperBound = INT_MAX;
    int lowerBound = INT_MIN;
	// count = 0;
	static int call_count = 0;

    while (lowerBound < upperBound) {
		call_count++;
        if (g == lowerBound) {
            beta = g + 1;
        } else {
            beta = g;
        }

        // g = minimax_v2(map, depth, true, beta - 1, beta);

        if (g < beta) {
            upperBound = g;
        } else {
            lowerBound = g;
        }
    }

    return g;
}


int Algo::minimax_v2(char *map, int depth, bool EstMax, int alpha, int beta, char joueur, char adversaire) {
    // je veux une variable globale qui compte le nombre de fois que la fonction est appelée
    static int call_count = 0;
    call_count++;

	// Generate hash key for the current position
    int hash_key = std::hash<std::string>{}(std::string(map, N * N));

    // Check if the position is in the transposition table
    TranspositionTable::TableEntry* entry = transposition_table.get(hash_key);
    if (entry != nullptr && entry->depth >= depth) {
        if (entry->flag == TranspositionTable::EXACT) {
            return entry->value;
        } else if (entry->flag == TranspositionTable::LOWERBOUND && entry->value > alpha) {
            alpha = entry->value;
        } else if (entry->flag == TranspositionTable::UPPERBOUND && entry->value < beta) {
            beta = entry->value;
        }

        if (alpha >= beta) {
            return entry->value;
        }
    }


    if (test_main(map, N, true, 0, 0) == INT_MAX) {
        return INT_MAX;
    }

    else if (test_main(map, N, false, 0, 0) == INT_MIN) {
        return INT_MIN;
    }

    if (depth == 0) {
        return test_main(map, N, true, 0, 0);
    }

    int meilleur_score;
    if (EstMax) {
        meilleur_score = INT_MIN;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (map[i * N + j] == '0') {
                    map[i * N + j] = joueur; // 2 pour le moment
                    int score = minimax_v2(map, depth - 1, false, alpha, beta, joueur, adversaire);
                    map[i * N + j] = '0';
					if (score - alpha >= SCORE_THRESHOLD) {
                        return score;
                    }
                    meilleur_score = std::max(meilleur_score, score);
                    alpha = std::max(alpha, meilleur_score);
                    if (beta <= alpha) { // Élagage bêta
                        return meilleur_score;
                    }
                }
            }
        }
    }
    else {
        meilleur_score = INT_MAX;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (map[i * N + j] == '0') {
                    map[i * N + j] = adversaire; // 1 pour le moment
                    int score = minimax_v2(map, depth - 1, true, alpha, beta, joueur, adversaire);
                    map[i * N + j] = '0';
					if (beta - score >= SCORE_THRESHOLD) {
                        return score;
                    }
                    meilleur_score = std::min(meilleur_score, score);
                    beta = std::min(beta, meilleur_score);
                    if (beta <= alpha) { // Élagage alpha
                        return meilleur_score;
                    }
                }
            }
        }
    }
	int flag;
    if (meilleur_score <= alpha) {
        flag = TranspositionTable::UPPERBOUND;
    } else if (meilleur_score >= beta) {
        flag = TranspositionTable::LOWERBOUND;
    } else {
        flag = TranspositionTable::EXACT;
    }
    transposition_table.store(hash_key, meilleur_score, depth, flag);
    return meilleur_score;
}

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


int Algo::coup(char *map, bool joueur) {
    dprintf(1, "joueur = %d", joueur);
    int mapSize = N;
    int meilleurScore = INT_MIN;
    int meilleurX = -1;
    int meilleurY = -1;
    int profondeurMax = 6; // Ajustez la profondeur maximale selon vos besoins

    // Utiliser les positions adjacentes
    std::vector<std::pair<int, int>> adjacent_positions = get_adjacent_positions(map, mapSize);

    if (adjacent_positions.empty()) {
        // S'il n'y a pas de pions sur le plateau, jouer au centre du plateau
        int center = mapSize / 2;
        map[center * mapSize + center] = joueur ? '2' : '1';
        return center * mapSize + center;
    }

    for (int profondeur = 1; profondeur <= profondeurMax; profondeur++) {
        int current_best_score = INT_MIN;
        int current_best_x = -1;
        int current_best_y = -1;

        for (auto &pos : adjacent_positions) {
            int i = pos.first;
            int j = pos.second;

            if (map[i * mapSize + j] == '0') {
                map[i * mapSize + j] = joueur ? '2' : '1';

                int score = minimax_v2(map, profondeur - 1, !joueur, INT_MIN, INT_MAX, joueur ? '2' : '1', joueur ? '1' : '2');
                if (score > current_best_score) {
                    current_best_score = score;
                    current_best_x = i;
                    current_best_y = j;
                }

                // Réinitialiser la position pour le prochain essai
                map[i * mapSize + j] = '0';
            }
        }

        if (current_best_score > meilleurScore) {
            meilleurScore = current_best_score;
            meilleurX = current_best_x;
            meilleurY = current_best_y;
        }

        // Si vous avez atteint un score maximal ou minimal, arrêtez l'Iterative Deepening
        if (meilleurScore == INT_MAX || meilleurScore == INT_MIN) {
            break;
        }
    }

    map[meilleurX * mapSize + meilleurY] = joueur ? '2' : '1';
    return meilleurScore;
}

