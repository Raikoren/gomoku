#include "algo.hpp"

int Algo::heuristique(const std::string& map, bool turn) {
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
    result = minMax(data.map, INT_MAX, INT_MIN, DEPTH, data.turn);
	// historique.clear();
	// historique.resize(0);
	movesOrder.clear();

	dprintf(1, "result = %d\n", result);
	return result;
}

int Algo::minMax(const std::string& position, int alpha, int beta, int depth, bool turn) {
	static int i = 0;
    if (depth == 0 || bScore == 5 || wScore == 5){
		// dprintf(1, "i = %d\n", i);
		i ++;
        return heuristique(position, turn); // return current score
	}
    std::vector<int> moves = setMovesOrder(position, turn);
	// dprintf(1, "moves.size() = %lu\n", moves.size());
    // for (int move : moves)
    //     std::cout << move << std::endl;
    if (turn) {
       int maxEval = -std::numeric_limits<float>::infinity();
       int child = 0;
       for (int move : moves) {
			std::string updatedMap = position;

			// Modifiez updatedMap pour inclure le mouvement en cours
			updatedMap[move] = turn ? '1' : '2';

			// Passez updatedMap directement à minMax
			int eval = minMax(updatedMap, alpha, beta, depth - 1, !turn);

			maxEval = std::max(maxEval, eval);
			alpha = std::max(alpha, maxEval);
       }
	   moves.clear();
       return maxEval;
    }
    else {
       int minEval = std::numeric_limits<float>::infinity();
       int child = 0;
       for (int move : moves) {
			std::string updatedMap = position;

			// Modifiez updatedMap pour inclure le mouvement en cours
			updatedMap[move] = turn ? '1' : '2';

			// Passez updatedMap directement à minMax
			int eval = minMax(updatedMap, alpha, beta, depth - 1, !turn);

			minEval = std::min(minEval, eval);
			beta = std::min(beta, minEval);
       }
	   moves.clear();
       return minEval;
    }
	moves.clear();
    return 0;
}

std::vector<int> Algo::setMovesOrder(const std::string& i, bool turn) {
    std::string         pos = i;
    int                 round = 0;
    int                 line = 0;
    int                 cell = 0;
    bool                firstRound = true;
    std::vector<int>    res;

    while (round < size) { // boucle pour chaques "anneaux" autour du dernier pion vraiment plac�
        int x = lastPoundX - (round + 1);
        int y = lastPoundY - (round + 1);
        while (line < 4) { // boucle pour chacune des 4 lignes composant un anneau
            int dx = line % 2 == 0 ? 1 : 0;
            int dy = dx == 0 ? 1 : 0;
            dx *= line == 2 ? -1 : 1;
            dy *= line == 3 ? -1 : 1;
            while (cell < 2 + (2 * round)) {
                if (!(x < 0 || x >= size || y < 0 || y >= size || (dx == 0 && dy == 0)) && checkPos(x, y, pos, firstRound, turn)) {
                    res.push_back(y * size + x);
                }
                x += dx;
                y += dy;
                cell++;
            }
            line++;
            cell = 0;
        }
        round++;
        line = 0;
        if (firstRound && round == size) {
            firstRound = false;
            round = 0;
        }
    }

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