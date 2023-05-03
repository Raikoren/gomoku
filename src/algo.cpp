#include "algo.hpp"

int Algo::heuristique(const std::string& map, bool turn) {
	static int i = 100;
	// dprintf(1, "heuristique\n");
	// dprintf(1, "map: %s\n", map.c_str());
	int score = 0;
	// return un random de 1 a 100
    return std::rand() % 100 + 1;
}

int Algo::ask(AlgoData data) {
    wScore = data.wScore;
    bScore = data.bScore;
    // historique.push_back(data.map);
    size = data.size;
    lastPoundY = data.lastPound / data.size;
    lastPoundX = data.lastPound % data.size;
    std::cout << data.lastPound << " " << lastPoundX << " " << lastPoundY << std::endl;

	int result = 0;
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    result = minMax(data.map, INT_MIN, INT_MAX, DEPTH, data.turn);
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	dprintf(1, "Time difference (milliseconds) = %lld\n", std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());
	// historique.clear();
	// historique.resize(0);
	movesOrder.clear();

	return result;
}

int Algo::minMax(const std::string& position, int alpha, int beta, int depth, bool turn) {
	static int i = 0;
	int prise;
    if (depth == 0 || bScore == 5 || wScore == 5){
		// dprintf(1, "i = %d\n", i);
		i ++;
        return heuristique(position, turn); // return current score
	}
    std::vector<int> moves = setMovesOrder(position, turn);

	// Prendre les 20% premiers mouvements
	if (moves.size() > 20){
		int movesToTake = std::ceil(moves.size() * 0.2);
		moves.resize(movesToTake);
	}


    // for (int move : moves)
    //     std::cout << move << std::endl;
	// dprintf(1, "nb moves: %d\n", moves.size());
    if (turn) {
       int maxEval = INT_MIN;
       int child = 0;
       for (int move : moves) {
			std::string updatedMap = position;

			// Modifiez updatedMap pour inclure le mouvement en cours
			updatedMap[move] = turn ? '1' : '2';

			// Passez updatedMap directement à minMax
			int eval = minMax(updatedMap, alpha, beta, depth - 1, !turn);

			maxEval = std::max(maxEval, eval);
			alpha = std::max(alpha, maxEval);
			if (beta <= alpha){
				break;
			}
       }
	//    moves.clear();
       return maxEval;
    }
    else {
       int minEval = INT_MAX;
       int child = 0;
       for (int move : moves) {
			std::string updatedMap = position;

			// Modifiez updatedMap pour inclure le mouvement en cours
			updatedMap[move] = turn ? '1' : '2';

			// Passez updatedMap directement à minMax
			int eval = minMax(updatedMap, alpha, beta, depth - 1, !turn);

			minEval = std::min(minEval, eval);
			beta = std::min(beta, minEval);
			if (beta <= alpha){
				break;
			}
       }
	//    moves.clear();
       return minEval;
    }
	// moves.clear();
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
    static bool firstRound = true;
    if (firstRound) {
        firstRound = false;
        res.push_back(size * size / 2);
    }

    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            if (pos[y * size + x] != '0') {
                for (int dy = -1; dy <= 1; ++dy) {
                    for (int dx = -1; dx <= 1; ++dx) {
                        int newX = x + dx;
                        int newY = y + dy;

                        if (newX < 0 || newX >= size || newY < 0 || newY >= size) {
                            continue;
                        }

                        if (checkPos(newX, newY, pos, firstRound, turn)) {
                            res.push_back(newY * size + newX);
                        }
                    }
                }
            }
        }
    }

    // Supprimez les doublons de la liste des mouvements
    std::sort(res.begin(), res.end());
    res.erase(std::unique(res.begin(), res.end()), res.end());

    return res;
}


bool Algo::checkPos(int x, int y, std::string map, bool firstRound, bool turn) {
    bool first = false;
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
            if (threeLine(dx, dy, x, y, map, turn)) {
                if (!first)
                    first = true;
                else
                    return false;
            }
        }
    }
    return firstRound == first ? true : false;
}

bool Algo::threeLine(int dx, int dy, int x, int y, const std::string map, bool turn) {
    char    p = turn ? '2' : '1';
    char    e = (p == '1') ? '2' : '1';
    int     hole = 0;
    int     ally = 0;
    bool    ennemy = false;
    int     epos;

    for (int i = 1; i < 5; i++) {
        if (x + (dx * i) < 0 || x + (dx * i) >= size || y + (dy * i) < 0 || y + (dy * i) >= size
            || map[((y + (dy * i)) * size) + (x + (dx * i))] == e) {
            ennemy = true;
            epos = i - 1;
            break;
        }
        if (map[((y + (dy * i)) * size) + (x + (dx * i))] == '0') {
            hole++;
            if (ally == 2 || hole == 2)
                break;
        }
        if (map[((y + (dy * i)) * size) + (x + (dx * i))] == p) {
            if (ally == 2)
                return false;
            ally++;
        }
    }
    if (ennemy && map[(y * size + ((dy * size) * epos)) + x + (dx * epos)] == p)
        return(false);
    dy *= -1;
    dx *= -1;
    if (ally == 2) {
        if (map[(y * size + (dy * size)) + x + dx] == '0')
            return(true);
        return (false);
    }
    if (ally == 1
        && x + dx >= 0 && x + dx < size && x + (dx * 2) >= 0 && x + (dx * 2) < size
        && y + dy >= 0 && y + dy < size && y + (dy * 2) >= 0 && y + (dy * 2) < size
        && map[((y + dy) * size) + x + dx] == p
        && map[((y + (dy * 2)) * size) + (x + (dx * 2))] == '0') {
        if (hole == 2 || (hole == 1 && (dy == -1 || (dy == 0 && dx == -1))))
            return(true);
    }
    return false;
}

bool Algo::canTake(int x, int y, std::string map, bool turn) {
    char e = turn ? '1' : '2';
    char a = !turn ? '1' : '2';
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (x + (dx * 3) < size && y + (dy * 3) < size
                && x + (dx * 3) >= 0 && y + (dy * 3) >= 0
                && map[(y + (dy * 1)) * size + (x + (dx * 1))] == e
                && map[(y + (dy * 2)) * size + (x + (dx * 2))] == e
                && map[(y + (dy * 3)) * size + (x + (dx * 3))] == a)
                return true;
        }
    }
    return false;
}