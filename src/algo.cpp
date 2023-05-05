#include "algo.hpp"

std::pair<int, int> Algo::FindPatternBothPlayers(const std::string &line) {
    std::pair<int, int> scores = std::make_pair(0, 0);

	auto it = transpositionTable_Line.find(line);
    if (it != transpositionTable_Line.end()) {
        return it->second;
    }

    // Calculer les scores pour les deux joueurs sans répéter les recherches
    int nb_pion_blanc = std::count(line.begin(), line.end(), '1');
    int nb_pion_noir = std::count(line.begin(), line.end(), '2');

	std::string patterns_blanc[] = {LiveTwo_Blanc, LiveTwo_Blanc2, LiveTwo_Blanc2_2, LiveTwo_Blanc3, LiveTwo_Blanc4};
	std::string patterns_noir[] = {LiveTwo_Noir, LiveTwo_Noir2, LiveTwo_Noir2_2, LiveTwo_Noir3, LiveTwo_Noir4};


    if (nb_pion_blanc >= 5 && line.find(FiveInRow_Blanc) != -1) {
		nb_pion_blanc -= 5;
        scores.first += 100000;
    }
    if (nb_pion_noir >= 5 && line.find(FiveInRow_Noir) != -1) {
		nb_pion_noir -= 5;
        scores.second += 100000;
    }

    if (nb_pion_blanc >= 4 && line.find(LiveFour_Blanc) != -1) {
		nb_pion_blanc -= 4;
        scores.first += 15000;
    }
    if (nb_pion_noir >= 4 && line.find(LiveFour_Noir) != -1) {
		nb_pion_noir -= 4;
        scores.second += 15000;
    }

    if (nb_pion_blanc >= 3 && line.find(LiveThree_Blanc) != -1) {
		nb_pion_blanc -= 3;
        scores.first += 3000;
    }
    if (nb_pion_noir >= 3 && line.find(LiveThree_Noir) != -1) {
		nb_pion_noir -= 3;
        scores.second += 3000;
    }

   
	if (nb_pion_blanc >= 2) {
		for (const auto& pattern : patterns_blanc) {
			if (line.find(pattern) != std::string::npos) {
				nb_pion_blanc -= 2;
				scores.first += 1000;
			}
			if (nb_pion_blanc < 2)
				break;
		}
	}

	if (nb_pion_blanc >= 2) {
		for (const auto& pattern : patterns_noir) {
			if (line.find(pattern) != std::string::npos) {
				nb_pion_noir -= 2;
				scores.second += 1000;
			}
			if (nb_pion_noir < 2)
				break;
		}
	}

	transpositionTable_Line[line] = scores;
    return scores;
}


bool Algo::fiveInRow(const std::string& map, bool turn, char player) {
    int count = 0;

    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            if (map[y * size + x] == player) {
                for (int dy = -1; dy <= 1; ++dy) {
                    for (int dx = -1; dx <= 1; ++dx) {
                        if (dx == 0 && dy == 0) {
                            continue;
                        }

                        int newX = x + dx * 4;
                        int newY = y + dy * 4;

                        if (newX < 0 || newX >= size || newY < 0 || newY >= size || map[newY * size + newX] != player) {
                            continue;
                        }

                        bool aligned = true;
                        for (int i = 1; i < 4; ++i) {
                            newX = x + dx * i;
                            newY = y + dy * i;

                            if (map[newY * size + newX] != player) {
                                aligned = false;
                                break;
                            }
                        }

                        if (aligned) {
                            return true;
                        }
                    }
                }
            }
        }
    }

    return false;
}

int Algo::heuristique(const std::string& map, bool turn) {
    int score = 0;
    char player = turn ? '2': '1';
    char opponent = turn ? '1': '2';

    for (int i = 0; i < size; i++) {
        std::string row = getRow(map, i);
        auto scores = FindPatternBothPlayers(row);
        score += scores.second - scores.first;
    }

    for (int i = 0; i < size; i++) {
        std::string col = getCol(map, i);
        auto scores = FindPatternBothPlayers(col);
        score += scores.second - scores.first;
    }
	
    for (int i = 0; i < size; i++) {
        std::string diagonal = getDiagonalFromRow(map, i);
        auto scores = FindPatternBothPlayers(diagonal);
        score += scores.second - scores.first;
    }

    for (int i = 0; i < size; i++) {
        std::string diagonal = getDiagonalFromCol(map, i);
        auto scores = FindPatternBothPlayers(diagonal);
        score += scores.second - scores.first;
    }

    return score;
}


std::string Algo::getRow(const std::string& map, int row) {
    return map.substr(row * size, size);
}

std::string Algo::getCol(const std::string& map, int col) {
    std::string column;
    for (int i = 0; i < size; i++) {
        column += map[col + i * size];
    }
    return column;
}

std::string Algo::getDiagonalFromRow(const std::string& map, int row) {
    std::string diagonal;
    for (int i = row, j = 0; i < size && j < size; i++, j++) {
        diagonal += map[i * size + j];
    }
    return diagonal;
}

std::string Algo::getDiagonalFromCol(const std::string& map, int col) {
    std::string diagonal;
    for (int i = 0, j = col; i < size && j < size; i++, j++) {
        diagonal += map[i * size + j];
    }
    return diagonal;
}



int Algo::ask(AlgoData data) {
    wScore = data.wScore;
    bScore = data.bScore;
    size = data.size;
	player_dark = data.turn ? true : false;

	int result = 0;
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    result = minMax(data.map, INT_MIN, INT_MAX, DEPTH, data.turn);
	printf("turn: %d\n", data.turn);
	printf("player: %d\n", player_dark);


	printf("result: %d\n", result);
	printf("optimalMove: %d\n", optimalMove);
	printf("optimalMove: %c, %d\n", optimalMove % size + 'A', size - (optimalMove / size));
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	printf("Time difference (milliseconds) = %lld\n", std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());
	// historique.clear();
	// historique.resize(0);
	movesOrder.clear();

	return optimalMove;
}

int Algo::minMax(const std::string& position, int alpha, int beta, int depth, bool turn) {
	static int i = 0;
	int prise;

	if (fiveInRow(position, turn, '2')) {
		return 100000 * depth;
	}
	else if (fiveInRow(position, turn, '1')) {
		return -100000 * depth;
	}

    if (depth == 0 || bScore == 5 || wScore == 5){
        return heuristique(position, turn); // return current score
	}

    std::vector<int> moves = setMovesOrder(position, turn);

	// Prendre les 50% premiers mouvements
	// if (moves.size() > 50){
	// 	int movesToTake = std::ceil(moves.size() * 0.5);
	// 	moves.resize(movesToTake);
	// }


    // for (int move : moves)
    //     std::cout << move << std::endl;
	// dprintf(1, "nb moves: %d\n", moves.size());
    if (turn) {
       int maxEval = INT_MIN;
       for (int move : moves) {
			i ++;
			std::string updatedMap = position;

			// Modifiez updatedMap pour inclure le mouvement en cours
			updatedMap[move] = '2';

			// Passez updatedMap directement à minMax
			int eval = minMax(updatedMap, alpha, beta, depth - 1, !turn);

			if (eval> maxEval){
				maxEval = eval;
				if (depth == DEPTH){
					optimalMove = move;
				}
			}
			alpha = std::max(alpha, maxEval);
			if (beta <= alpha){
				// dprintf(1, "beta <= alpha\n");
				// dprintf(1, "i = %d\n", i);
				// dprintf(1, "beta: %d\n", beta);
				// dprintf(1, "alpha: %d\n", alpha);
				break;
			}
       }
       return maxEval;
    }
    else {
       int minEval = INT_MAX;
       for (int move : moves) {
			std::string updatedMap = position;

			// Modifiez updatedMap pour inclure le mouvement en cours
			updatedMap[move] = '1';

			// Passez updatedMap directement à minMax
			int eval = minMax(updatedMap, alpha, beta, depth - 1, !turn);

			if (eval < minEval){
				minEval = eval;
				if (depth == DEPTH){
					optimalMove = move;
				}
			}
			beta = std::min(beta, minEval);
			if (beta <= alpha){
				break;
			}
       }
       return minEval;
    }
    return 0;
}

std::vector<std::pair<int, int>> Algo::getWindowBounds(const std::string& map) {
    int min_x = size, min_y = size, max_x = 0, max_y = 0;

    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            if (map[y * size + x] != '0') {
                min_x = std::min(min_x, x);
                min_y = std::min(min_y, y);
                max_x = std::max(max_x, x);
                max_y = std::max(max_y, y);
            }
        }
    }

    return {{min_x, min_y}, {max_x, max_y}};
}

std::vector<int> Algo::setMovesOrder(const std::string& i, bool turn) {
    std::string pos = i;
    std::vector<int> res;
    bool firstCheck = true;

    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            if (pos[y * size + x] != '0') {
                for (int dy = -1; dy <= 1; ++dy) {
                    for (int dx = -1; dx <= 1; ++dx) {
                        int newX = x + dx;
                        int newY = y + dy;
                        if (newX < 0 || newX >= size || newY < 0 || newY >= size || pos[newY * size + newX] != '0') {
                            continue;
                        }
                        if (std::find(res.begin(), res.end(), newY * size + newX) == res.end() && checkPos(newX, newY, pos, firstCheck, turn)) {
                            res.push_back(newY * size + newX);
                        }
                    }
                }
            }
        }
		if (y == size - 1 && firstCheck == false)
			break;
        if (y == size - 1) {
            firstCheck = false;
            y = 0;
        }
    }
    if (res.empty())
        res.push_back(size * size / 2);
    return res;
}


bool Algo::checkPos(int x, int y, std::string map, bool firstRound, bool turn) {
    bool first = false;
    bool important = false;
    if (map[y * size + x] != '0')
        return false;
    if (canTake(x, y, map, turn)) {
        return (firstRound);
    }
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dx == 0 && dy == 0) {
                continue;
            }
            if (takeAdvantage(dx, dy, x, y, map, turn) || threeLine(dx, dy, x + dx, y + dy, map, !turn)) {
                important = true;
            }
            if (threeLine(dx, dy, x, y, map, turn)) {
                if (!first)
                    first = true;
                else
                    return false;
            }
        }
    }
    if (important && firstRound)
        return true;
    return firstRound == first ? true : false;
}

bool Algo::threeLine(int dx, int dy, int x, int y, const std::string map, bool turn) {
    char    p = turn ? '2' : '1';
    char    e = (p == '1') ? '2' : '1';
    int     hole = 0;
    int     ally = 0;
    bool    safeBreak = false;
    bool    lastHitEmpty = false;
    int     ennemy;

    if (x < 0 || y < 0 || x >= size || y >= size) {
        return false;
    }
    for (int i = 1; i < 5 && !(x + dx * i < 0 || x + dx * i >= size || y + dy * i < 0 || y + dy * i >= size); i++) {
        lastHitEmpty = false;
        if (map[((dy * i + y) * size) + x + (dx * i)] == e) {
            if (map[((dy * (i - 1) + y) * size) + x + (dx * (i - 1))] == p)
                return false;
            ennemy = i - 1;
            safeBreak = true;
            break;
        }
        else if (map[((dy * i + y) * size) + x + (dx * i)] == '0') {
            hole++;
            lastHitEmpty = true;
            if (ally == 2 || hole == 2) {
                safeBreak = true;
                break;
            }
        }
        else if (map[((dy * i + y) * size) + x + (dx * i)] == p) {
            if (ally == 2)
                return false;
            ally++;
        }
    }
    dy *= -1;
    dx *= -1;
    if (!safeBreak && !lastHitEmpty)
        return false;
    if (ally == 2)
        return ((y + dy >= 0 && y + dy < size&& x + dx < size&& x + dx >= 0) && map[((y + dy) * size) + x + dx] == '0');
    else if (ally == 1 && dy != 1 && !(dx == 1 && dy == 0)
        && x + (dx * 2) >= 0 && x + (dx * 2) < size
        && y + (dy * 2) >= 0 && y + (dy * 2) < size
        && map[((y + dy) * size) + x + dx] == p
        && map[((y + (dy * 2)) * size) + (x + (dx * 2))] == '0')
        return(true);
    return false;
}

bool Algo::canTake(int x, int y, std::string map, bool turn) {
    char ennemy = turn ? '1' : '2';
    char ally = !turn ? '1' : '2';
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (x + (dx * 3) < size && y + (dy * 3) < size
                && x + (dx * 3) >= 0 && y + (dy * 3) >= 0
                && map[(y + (dy * 1)) * size + (x + (dx * 1))] == ennemy
                && map[(y + (dy * 2)) * size + (x + (dx * 2))] == ennemy
                && map[(y + (dy * 3)) * size + (x + (dx * 3))] == ally)
                return true;
        }
    }
    return false;
}

bool Algo::takeAdvantage(int dx, int dy, int x, int y, std::string map, bool turn) {
    int i = 1;
    int count = 0;
    char ennemy = turn ? '1' : '2';
    char ally = !turn ? '1' : '2';
    int  safeBreak = 0;
    while (x + dx * i >= 0 && y + dy * i >= 0 && x + dx * i < size && y + dy * i < size) {
        if (map[(y + dy * i) * size + (x + dx * i)] == ally)
            count++;
        if (map[(y + dy * i) * size + (x + dx * i)] == ennemy)
            return false;
        if (map[(y + dy * i) * size + (x + dx * i)] == '0') {
            dy *= -1;
            dx *= -1;
            safeBreak++;
            break;
        }
        i++;
    }
    i = 1;
    if (safeBreak == 1) {
        while (x + dx * i >= 0 && y + dy * i >= 0 && x + dx * i < size && y + dy * i < size) {
            if (map[(y + dy * i) * size + (x + dx * i)] == ally)
                count++;
            if (map[(y + dy * i) * size + (x + dx * i)] == ennemy)
                return false;
            if (map[(y + dy * i) * size + (x + dx * i)] == '0') {
                dy *= -1;
                dx *= -1;
                safeBreak++;
                break;
            }
            i++;
        }
    }
    return safeBreak == 2 && count == 3;
}