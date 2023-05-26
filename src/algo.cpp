#include "algo.hpp"

bool Algo::isValidCoordinate(int x, int y) {
    return x >= 0 && x < size && y >= 0 && y < size;
}


std::vector<int> Algo::getCaptureIndices(const std::string& map, int move, bool turn) {
    int dx[] = {-1, 1, 0, 0, -1, 1, -1, 1};
    int dy[] = {0, 0, -1, 1, -1, 1, 1, -1};
    char player = turn ? '2' : '1';
    char opponent = turn ? '1' : '2';
    int x = move % size;
    int y = move / size;

    std::vector<int> captureIndices;

    for (int i = 0; i < 8; i++) {
        int x1 = x + dx[i];
        int y1 = y + dy[i];
        int x2 = x1 + dx[i];
        int y2 = y1 + dy[i];
        int x3 = x2 + dx[i];
        int y3 = y2 + dy[i];

        if (isValidCoordinate(x1, y1) && isValidCoordinate(x2, y2) && isValidCoordinate(x3, y3)) {
            if (map[y1 * size + x1] == opponent && map[y2 * size + x2] == opponent && map[y3 * size + x3] == player) {
                captureIndices.push_back(y1 * size + x1);
                captureIndices.push_back(y2 * size + x2);
                return captureIndices;
            }
        }
    }

    return captureIndices;
}


// Struct pour stocker les motifs et les valeurs de score correspondantes.
struct PatternScore {
    std::vector<std::string> patterns;
    int score;
    int pions;
    bool isBlanc;
};

struct EdgePatternScore {
    std::vector<std::string> patterns;
    int score;
    int pions;
    bool isBlanc;
    int patternSize;
};

std::pair<int, int> Algo::FindPatternBothPlayers(const std::string &line) {
    std::pair<int, int> scores = std::make_pair(0, 0);

    auto it = transpositionTable_Line.find(line);
    if (it != transpositionTable_Line.end()) {
        return it->second;
    }

    int nb_pion_blanc = std::count(line.begin(), line.end(), '1');
    int nb_pion_noir = std::count(line.begin(), line.end(), '2');

    // Initialiser les motifs et les scores correspondants pour chaque type de motif.
    std::vector<PatternScore> patternScores = {
        {patterns_noir_LiveFour, 150000, 4, false}, {patterns_blanc_LiveFour, 150000, 4, true},
        {patterns_noir_DeadFour, 10000, 4, false}, {patterns_blanc_DeadFour, 10000, 4, true},
        {patterns_noir_LiveThree, 15000, 3, false}, {patterns_blanc_LiveThree, 15000, 3, true},
        {patterns_noir_DeadThree, 1000, 3, false}, {patterns_blanc_DeadThree, 1000, 3, true},
        {patterns_noir_LiveTwo, 5000, 2, false}, {patterns_blanc_LiveTwo, 5000, 2, true},
        {patterns_noir_DeadTwo, 500, 2, false}, {patterns_blanc_DeadTwo, 500, 2, true},
		// {patterns_eatnoir, -1000, false}, {patterns_eatblanc, -1000, true} // test
    };

	std::vector<EdgePatternScore> edgePatternScores = {
        {{Edge_Four_Blanc, Edge_Four_Blanc2}, 10000, 4, true, 5},
        {{Edge_Four_Noir, Edge_Four_Noir2}, 10000, 4, false, 5},
        {{Edge_Three_Blanc, Edge_Three_Blanc2}, 1000, 3, true, 4},
        {{Edge_Three_Noir, Edge_Three_Noir2}, 1000, 3, false, 4},
        {{Edge_Two_Blanc, Edge_Two_Blanc2}, 500, 2, true, 3},
        {{Edge_Two_Noir, Edge_Two_Noir2}, 500, 2, false, 3}
    };

	if (nb_pion_blanc >= 5 && line.find(FiveInRow_Blanc) != -1) {
		nb_pion_blanc -= 5;
        scores.first += 1000000;
		return scores;
    }
    if (nb_pion_noir >= 5 && line.find(FiveInRow_Noir) != -1) {
		nb_pion_noir -= 5;
        scores.second += 1000000;
		return scores;
    }

    // Traitement pour chaque modèle de bord.
    for (auto &eps : edgePatternScores) {
        int &nb_pion = eps.isBlanc ? nb_pion_blanc : nb_pion_noir;
        int &score = eps.isBlanc ? scores.first : scores.second;

        if (nb_pion >= eps.pions) {
            for (const auto& pattern : eps.patterns) {
                if (line.find(pattern) == 0 || line.find(pattern) == line.size() - eps.patternSize) {
                    nb_pion -= eps.pions;
                    score += eps.score;
                }
                if (nb_pion < eps.pions)
                    break;
            }
        }
    }


    // Traitement pour chaque type de motif.
    for (auto &ps : patternScores) {
        int &nb_pion = ps.isBlanc ? nb_pion_blanc : nb_pion_noir;
        int &score = ps.isBlanc ? scores.first : scores.second;

        if (nb_pion >= ps.pions) {
            for (const auto& pattern : ps.patterns) {
                if (line.find(pattern) != std::string::npos) {
                    nb_pion -= ps.pions;
                    score += ps.score;
                }
                if (nb_pion < ps.pions)
                    break;
            }
        }
    }

    transpositionTable_Line[line] = scores;
    return scores;
}





bool Algo::fiveInRow(const std::string& map, bool turn, char player) {
    const int directions[4][2] = {
        {0, 1},    // vertical
        {1, 0},    // horizontal
        {1, 1},    // diagonal vers le bas
        {1, -1}    // diagonal vers le haut
    };

    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            if (map[y * size + x] == player) {
                for (int i = 0; i < 4; ++i) { // parcours des 4 directions
                    int dx = directions[i][0];
                    int dy = directions[i][1];

                    // vérifie si la séquence de cinq est possible dans cette direction
                    if (x + 4 * dx >= 0 && x + 4 * dx < size && y + 4 * dy >= 0 && y + 4 * dy < size) {
                        bool aligned = true;
                        for (int j = 1; j <= 4; ++j) {
                            int newX = x + dx * j;
                            int newY = y + dy * j;
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


int Algo::heuristique(const std::string& map, bool turn, int bscore, int wscore) {
    int score = 0;

    // Si nous avons déjà calculé le score pour cette carte, retournez-le immédiatement.
    auto it = hashedTranspositionTableBoard.find(hasher(map));
    if (it != hashedTranspositionTableBoard.end()) {
        return it->second;
    }

    // Évitez de calculer ceci si c'est possible.
    if (bscore == 5 || wscore == 5 || fiveInRow(map, turn, '2') || fiveInRow(map, turn, '1')) {
        score = (bscore == 5 || fiveInRow(map, turn, '2')) ? 1000000 : -1000000;
        hashedTranspositionTableBoard[hasher(map)] = score;
        return score;
    }

    for (int i = 0; i < size; i++) {
        std::string rows[] = { getRow(map, i), getCol(map, i), getDiagonalFromRow(map, i), 
                               getDiagonalFromCol(map, i), getDiagonalSecondaryFromRow(map, i),
                               getDiagonalSecondaryFromCol(map, i) };

        for(auto& row : rows) {
            auto tmpScores = FindPatternBothPlayers(row);
            score += tmpScores.second - tmpScores.first;
        }
    }

    hashedTranspositionTableBoard[hasher(map)] = score;

    int score_adjustments[] = {5000, 8000, 10000, 13000};
    if (bscore > 0 && bscore <= 4) {
        score += score_adjustments[bscore - 1];
    }
    if (wscore > 0 && wscore <= 4) {
        score -= score_adjustments[wscore - 1];
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

std::string Algo::getDiagonalSecondaryFromRow(const std::string& map, int row) {
    std::string diagonal;
    for (int i = row, j = 0; i >= 0 && j < size; i--, j++) {
        diagonal += map[i * size + j];
    }
    return diagonal;
}

std::string Algo::getDiagonalSecondaryFromCol(const std::string& map, int col) {
    std::string diagonal;
    for (int i = size - 1, j = col; i >= 0 && j < size; i--, j++) {
        diagonal += map[i * size + j];
    }
    return diagonal;
}




int Algo::ask(AlgoData data) {
	eat_pion_blanc = false;
	eat_pion_noir = false;

    Map_wScore = data.wScore;
    Map_bScore = data.bScore;
    size = data.size;
	player_dark = data.turn ? true : false;
    countCheckedLine = 0;
    nbOfPruning = 0;
    timespentInSetMovesOrder = 0;
    timespentInHeuristic = 0 ;
    timespentToSearchInTranspoTable = 0;
    timespentInSomeFunctions = 0;

	int maxDepth = 10;
    int result = 0;
	int best_result = 0;
	int best_result_blanc = INT_MAX;
	int best_result_noir = INT_MIN;
	int best_move = 0;
	int best_alpha = INT_MIN;
	int best_beta = INT_MAX;
	hashedTranspositionTableBoard.clear();
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    for (int currentDepth = 4; currentDepth <= maxDepth; currentDepth++) {
		iterativeDepth = currentDepth;
		out_time = false;
		printf("currentDepth: %d\n", currentDepth);
        result = minMax(data.map, INT_MIN, INT_MAX, currentDepth, data.turn, &begin, Map_bScore, Map_wScore);
		if (out_time == true){
            printf("minMax failed\n");
			result = best_result;
			optimalMove = best_move;
		}

		// printf("optimalAlpha: %d\n", optimalAlpha);
		// printf("bestalpha: %d\n", best_alpha);
		// printf("optimalBeta: %d\n", optimalBeta);
		// printf("bestbeta: %d\n", best_beta);
		// printf("best_move: %c, %d\n", optimalMove % size + 'A', size - (optimalMove / size));

		// if (data.turn == true ){
		// 	if (best_alpha < optimalAlpha && optimalAlpha != INT_MIN) {
		// 		best_result = result;
		// 		best_move = optimalMove;
		// 		best_alpha = optimalAlpha;

		if (data.turn == false && result < best_result_blanc){ // TEST EN GARDANT LE MOVE AVEC LE PLUS DE SCORE
				// printf("result_blanc: %d\n", best_result_blanc);
				// printf("best_result: %d\n", best_result);
				best_result_blanc = result;
				best_result = result;
				best_move = optimalMove;
		}
		else if (data.turn == true && result > best_result_noir){
				// printf("result_noir: %d\n", best_result_noir);
				// printf("best_result: %d\n", best_result);
				best_result_noir = result;
				best_result = result;
				best_move = optimalMove;
		}
		// printf("\n\n");
		// printf("best_move: %c, %d\n", optimalMove % size + 'A', size - (optimalMove / size));
		// printf("best_result: %d\n", result);
		// printf("\n\n");


		
        printf("size of transposition table line %lld\n",transpositionTable_Line.size());
        printf("size of transposition table board%lld\n",hashedTranspositionTableBoard.size());
		std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - begin).count();
		
		//test time
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		auto elapsed_ms2 = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
		printf("Time difference (milliseconds) = %lld\n", elapsed_ms2);

		if (elapsed_ms2 > 500){
			// printf("time out\n");
			break;
		}
    }

	if (best_result == 0){
		best_result = result;
		best_move = optimalMove;
	}
	else {
		result = best_result;
		optimalMove = best_move;
	}



	// printf("result: %d\n", result);
	// printf("optimalMove: %d\n", optimalMove);
	// printf("optimalMove: %c, %d\n", optimalMove % size + 'A', size - (optimalMove / size));
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	printf("Time difference (milliseconds) = %lld\n", std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());
	printf("     =========================\n\n");
    printf("Lines checked: %d\n",countCheckedLine);
    printf("Prunes done: %d\n",nbOfPruning);

    // printf("timespentInMovesOrder: %lld\n",timespentInSetMovesOrder);
    // printf("timespentInHeuristic: %lld\n",timespentInHeuristic);
    // printf("timespentInSearchTableLine: %lld\n",timespentToSearchInTranspoTable);
    // printf("timespentInSearchTableLine: %lld\n",timespentInSomeFunctions);
	
    // historique.clear();
	// historique.resize(0);
	movesOrder.clear();

	return optimalMove;
}

int Algo::minMax(const std::string& position, int alpha, int beta, int depth, bool turn, std::chrono::steady_clock::time_point* begin, int bScore, int wScore) {
    countCheckedLine++;
    // if(depth>3)printf("startedAlgoAtDepth: %d\n",depth);
    // Vérifier le temps écoulé
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - *begin).count();
    if (elapsed_ms > 500) {
		out_time = true;
        return -1;
	}
	// 	if (fiveInRow(position, turn, '2')) {
	// 		return 10000000;
	// 	}
	// 	else if (fiveInRow(position, turn, '1')) {
	// 		return -10000000;
	// 	}
    //     return heuristique(position, turn, bScore, wScore);
    // }

    if (fiveInRow(position, turn, '2') || bScore == 5) {
        return 10000000 * depth;
    }
    else if (fiveInRow(position, turn, '1') || wScore == 5) {
        return -10000000 * depth;
    }

    if (depth == 0){
        return heuristique(position, turn, bScore, wScore); // return current score
    }

    // std::vector<int> moves = setMovesOrder(position, turn);
    std::vector<int> moves = setMovesOrderLineScore(position, turn);

    if (turn) {
        int maxEval = INT_MIN;
        for (int move : moves) {
			eat_pion_blanc = false;
            std::string updatedMap = position;

            // Modifiez updatedMap pour inclure le mouvement en cours
            updatedMap[move] = '2';



            std::vector<int> captured = getCaptureIndices(updatedMap, move, turn);

            if (!captured.empty()) {
				eat_pion_blanc = true;
                updatedMap[captured[0]] = '0';
                updatedMap[captured[1]] = '0';
                bScore++;
            }

            // Passez updatedMap directement à minMax
            int eval = minMax(updatedMap, alpha, beta, depth - 1, !turn, begin, bScore, wScore);

			if (!captured.empty()) {
				bScore--;
			}
			alpha = std::max(alpha, maxEval);
            if (eval > maxEval){
                maxEval = eval;
                if (depth == iterativeDepth){
					// printf("eval: %d\n", eval);
					alpha = std::max(alpha, maxEval);

                    optimalMove = move;
                    optimalAlpha = alpha;
                    optimalBeta = beta;
                }
            }
            if (beta <= alpha){
                // if(depth>3)printf("prune done at depth%d\n",depth);
                nbOfPruning++;
                break;
            }
        }
        // if(depth == iterativeDepth) printf("minMax Ended\n");
        return maxEval;
    }
    else {
        int minEval = INT_MAX;
        for (int move : moves) {
			eat_pion_noir = false;
            std::string updatedMap = position;

            // Modifiez updatedMap pour inclure le mouvement en cours
            updatedMap[move] = '1';

            std::vector<int> captured = getCaptureIndices(updatedMap, move, turn);
            if (!captured.empty()) {
				eat_pion_noir = true;
                updatedMap[captured[0]] = '0';
                updatedMap[captured[1]] = '0';
                wScore++;
            }

            // Passez updatedMap directement à minMax
            int eval = minMax(updatedMap, alpha, beta, depth - 1, !turn, begin, bScore, wScore);

			if (!captured.empty()) {
				wScore--;
			}
			beta = std::min(beta, minEval);
            if (eval < minEval){
                minEval = eval;
                if (depth == iterativeDepth){
					beta = std::min(beta, minEval);

                    optimalMove = move;
                    optimalAlpha = alpha;
                    optimalBeta = beta;
                }
            }
			if (beta <= alpha){
                // if(depth>3)printf("prune done at depth%d\n",depth);
                nbOfPruning++;
				break;
			}
       }
        if(depth == iterativeDepth) printf("minMax Ended\n");
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

std::vector<int> Algo::setMovesOrderLineScore(const std::string& map, bool turn){
    std::set<int> result_set;
    std::set<int> temp_result_set;
    char player = turn ? '2': '1';
    char opponent = turn ? '1': '2';

    for (int y = 1; y < size - 1; ++y) {
        for (int x = 1; x < size - 1; ++x) {
            if (map[y * size + x] != '0') {
                for (int dy = -1; dy <= 1; ++dy) {
                    for (int dx = -1; dx <= 1; ++dx) {
                        int newX = x + dx;
                        int newY = y + dy;
                        if (map[newY * size + newX] != '0' || newX < 0 || newX >= size || newY < 0 || newY >= size) {
                            continue;
                        }
                        if (checkGoodPos(map, newY, newX, turn)) {
                            result_set.insert(newY * size + newX);
                        }
                        temp_result_set.insert(newY * size + newX);
                    }
                }
            }
        }
    }

    if (result_set.empty()) {
        result_set = std::move(temp_result_set);
    }

    return std::vector<int>(result_set.begin(), result_set.end());
}



bool Algo::checkGoodPos(const std::string& mapWithIncomingNewMove, int newY, int newX, bool turn){
    char player = turn ? '2' : '1';
    char opponent = turn ? '1' : '2';
	int empty = 0;
    
    std::string map = mapWithIncomingNewMove;
	map[newY * size + newX] = player;

    int dx[] = {0, 1, 1, 1, 0, -1, -1, -1};
    int dy[] = {-1, -1, 0, 1, 1, 1, 0, -1};
    
    for (int dir = 0; dir < 8; dir++) {
        int count_p = 0;
		int count_o = 0;
        for (int dist = 1; dist <= 2; dist++) {
            int x = newX + dx[dir] * dist;
            int y = newY + dy[dir] * dist;
            if (x < 0 || x >= size || y < 0 || y >= size) {
                break;
            }
            if (count_o == 0 && mapWithIncomingNewMove[y * size + x] == player) {
                count_p++;
            }
			else if (count_p == 0 && mapWithIncomingNewMove[y * size + x] == opponent) {
				count_o++;
			}
			else if (empty == 0){
				empty++;
			}
			else {
				break;
            }
        }
        if (count_p == 2 || count_o == 2) {
            return true;
        }
    }
    return false;
}

int Algo::checkScorePos(string mapWithIncomingNewMove, int newY, int newX, bool turn){
    int score = 0;
    mapWithIncomingNewMove[newY * size + newX] = turn ? '2': '1';

    // auto it = transpositionTableBoard.find(mapWithIncomingNewMove);
    // if (it != transpositionTableBoard.end()) {
    //     // printf("already found %s in table, returning score\n",line);
    //     // printf("time to find in transposition table%lld\n",end1-begin1);
    //     return it->second;
    // }
    std::string row = getRow(mapWithIncomingNewMove, newY);
    auto tmpScores = FindPatternBothPlayers(row);
    score += tmpScores.second - tmpScores.first;

    std::string col = getCol(mapWithIncomingNewMove, newX);
    tmpScores = FindPatternBothPlayers(col);
    score += tmpScores.second - tmpScores.first;

    std::string diagonal = getDiagonalFromRow(mapWithIncomingNewMove, newY);
    tmpScores = FindPatternBothPlayers(diagonal);
    score += tmpScores.second - tmpScores.first;

    diagonal = getDiagonalFromCol(mapWithIncomingNewMove, newX);
    tmpScores = FindPatternBothPlayers(diagonal);
    score += tmpScores.second - tmpScores.first;

    diagonal = getDiagonalSecondaryFromRow(mapWithIncomingNewMove, newY);
	tmpScores = FindPatternBothPlayers(diagonal);
	score += tmpScores.second - tmpScores.first;

	diagonal = getDiagonalSecondaryFromCol(mapWithIncomingNewMove, newX);
	tmpScores = FindPatternBothPlayers(diagonal);
	score += tmpScores.second - tmpScores.first;

    return score;
}

std::vector<int> Algo::setMovesOrder(const std::string& i, bool turn) {
    std::string pos = i;
    std::vector<int> res;
    // std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            if (pos[y * size + x] != '0') {
                for (int dy = -2; dy <= 2; ++dy) {
                    for (int dx = -2; dx <= 2; ++dx) {
                        int newX = x + dx;
                        int newY = y + dy;
                        if (newX < 0 || newX >= size || newY < 0 || newY >= size || pos[newY * size + newX] != '0') {
                            continue;
                        }
                        if (std::find(res.begin(), res.end(), newY * size + newX) == res.end() && checkPos(newX, newY, pos, turn)) {
                            res.push_back(newY * size + newX);
                        }
                    }
                }
            }
        }
    }
    if (res.empty())
        res.push_back(size * size / 2);
    // std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    // timespentInSetMovesOrder += std::chrono::duration_cast<std::chrono::milliseconds>(end.time_since_epoch()).count() - std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()).count();
    return res;
}


bool Algo::checkPos(int x, int y, std::string map, bool turn) {
    bool first = false;
    bool important = false;
    if (map[y * size + x] != '0')
        return false;
    if (canTake(x, y, map, turn)) {
        return (true);
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
    return true;
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