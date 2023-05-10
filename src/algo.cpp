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




std::pair<int, int> Algo::FindPatternBothPlayers(const std::string &line) {
    std::pair<int, int> scores = std::make_pair(0, 0);

    std::chrono::steady_clock::time_point begin1 = std::chrono::steady_clock::now();
	auto it = transpositionTable_Line.find(line);
    std::chrono::steady_clock::time_point endFind = std::chrono::steady_clock::now();
    timespentToSearchInTranspoTable += std::chrono::duration_cast<std::chrono::milliseconds>(endFind.time_since_epoch()).count() - std::chrono::duration_cast<std::chrono::milliseconds>(begin1.time_since_epoch()).count();
    if (it != transpositionTable_Line.end()) {
        // printf("already found %s in table, returning score\n",line);
        // printf("time to find in transposition table%lld\n",end1-begin1);
        return it->second;
    }
    // cout << line << " is a new line" << endl;
    // Calculer les scores pour les deux joueurs sans répéter les recherches
    int nb_pion_blanc = std::count(line.begin(), line.end(), '1');
    int nb_pion_noir = std::count(line.begin(), line.end(), '2');

	std::string patterns_blanc_DeadFour[] = {DeadFour_Blanc, DeadFour_Blanc_1, DeadFour_Blanc_2,DeadFour_Blanc1_1, DeadFour_Blanc2, DeadFour_Blanc2_2, DeadFour_Blanc3};
	std::string patterns_blanc_LiveFour[] = {LiveFour_Blanc, LiveFour_Blanc_1, LiveFour_Blanc_2};

	std::string patterns_blanc_DeadThree[] = {DeadThree_Blanc, DeadThree_Blanc_1, DeadThree_Blanc_2 ,DeadThree_Blanc1_1, DeadThree_Blanc2, DeadThree_Blanc2_2, DeadThree_Blanc3, DeadThree_Blanc3_2, DeadThree_Blanc4};
	std::string patterns_blanc_LiveThree[] = {LiveThree_Blanc, LiveThree_Blanc_1, LiveThree_Blanc_2};

	std::string patterns_blanc_LiveTwo[] = {LiveTwo_Blanc, LiveTwo_Blanc2, LiveTwo_Blanc2_2, LiveTwo_Blanc3, LiveTwo_Blanc4};
	std::string patterns_blanc_DeadTwo[] = {DeadTwo_Blanc, DeadTwo_Blanc1_1, DeadTwo_Blanc2, DeadTwo_Blanc2_2, DeadTwo_Blanc3};

	std::string patterns_noir_DeadFour[] = {DeadFour_Noir, DeadFour_Blanc_1, DeadFour_Blanc_2,DeadFour_Noir1_1, DeadFour_Noir2, DeadFour_Noir2_2, DeadFour_Noir3};
	std::string patterns_noir_LiveFour[] = {LiveFour_Noir, LiveFour_Noir_1, LiveFour_Noir_2};

	std::string patterns_noir_DeadThree[] = {DeadThree_Noir, DeadThree_Noir_1, DeadThree_Noir_2,DeadThree_Noir1_1, DeadThree_Noir2, DeadThree_Noir2_2, DeadThree_Noir3, DeadThree_Noir3_2, DeadThree_Noir4};
	std::string patterns_noir_LiveThree[] = {LiveThree_Noir, LiveThree_Noir_1, LiveThree_Noir_2};

	std::string patterns_noir_LiveTwo[] = {LiveTwo_Noir, LiveTwo_Noir2, LiveTwo_Noir2_2, LiveTwo_Noir3, LiveTwo_Noir4};
	std::string patterns_noir_DeadTwo[] = {DeadTwo_Noir, DeadTwo_Noir1_1, DeadTwo_Noir2, DeadTwo_Noir2_2, DeadTwo_Noir3};

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
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

	if (nb_pion_blanc >= 4) {
		for (const auto& pattern : patterns_blanc_LiveFour) {
			if (line.find(pattern) != std::string::npos) {
				nb_pion_blanc -= 4;
				scores.first += 15000;
			}
			if (nb_pion_blanc < 4)
				break;
		}
	}

	if (nb_pion_blanc >= 4) {
		for (const auto& pattern : patterns_blanc_DeadFour) {
			if (line.find(pattern) != std::string::npos) {
				nb_pion_blanc -= 4;
				scores.first += 10000;
			}
			if (nb_pion_blanc < 4)
				break;
		}
	}
	if (nb_pion_blanc >= 4 && (line.find(Edge_Four_Blanc) == 0)
	|| nb_pion_blanc >= 4 && (line.find(Edge_Four_Blanc2) == line.size() - 5)) {
		nb_pion_blanc -= 4;
        scores.first += 10000;
    }

	if (nb_pion_noir >= 4) {
		for (const auto& pattern : patterns_noir_LiveFour) {
			if (line.find(pattern) != std::string::npos) {
				nb_pion_noir -= 4;
				scores.second += 15000;
			}
			if (nb_pion_noir < 4)
				break;
		}
	}
	if (nb_pion_noir >= 4) {
		for (const auto& pattern : patterns_noir_DeadFour) {
			if (line.find(pattern) != std::string::npos) {
				nb_pion_noir -= 4;
				scores.second += 10000;
			}
			if (nb_pion_noir < 4)
				break;
		}
	}
	if (nb_pion_noir >= 4 && (line.find(Edge_Four_Noir) == 0) ||
		nb_pion_noir >= 4 && (line.find(Edge_Four_Noir2) == line.size() - 5)) {
		nb_pion_noir -= 4;
		scores.second += 10000;
	}

	if (nb_pion_blanc >= 3) {
		for (const auto& pattern : patterns_blanc_LiveThree) {
			if (line.find(pattern) != std::string::npos) {
				nb_pion_blanc -= 3;
				scores.first += 10000;
			}
			if (nb_pion_blanc < 3)
				break;
		}
	}
	if (nb_pion_blanc >= 3) {
		for (const auto& pattern : patterns_blanc_DeadThree) {
			if (line.find(pattern) != std::string::npos) {
				nb_pion_blanc -= 3;
				scores.first +=1000;
			}
			if (nb_pion_blanc < 3)
				break;
		}
	}
	if (nb_pion_blanc >= 3 && (line.find(Edge_Three_Blanc) == 0) ||
		nb_pion_blanc >= 3 && (line.find(Edge_Three_Blanc2) == line.size() - 4)) {
		nb_pion_blanc -= 3;
		scores.first += 1000;
	}

	if (nb_pion_noir >= 3) {
		for (const auto& pattern : patterns_noir_LiveThree) {
			if (line.find(pattern) != std::string::npos) {
				nb_pion_noir -= 3;
				scores.second += 10000;
			}
			if (nb_pion_noir < 3)
				break;
		}
	}
	if (nb_pion_noir >= 3) {
		for (const auto& pattern : patterns_noir_DeadThree) {
			if (line.find(pattern) != std::string::npos) {
				nb_pion_noir -= 3;
				scores.second += 1000;
			}
			if (nb_pion_noir < 3)
				break;
		}
	}
	if (nb_pion_noir >= 3 && (line.find(Edge_Three_Noir) == 0) ||
		nb_pion_noir >= 3 && (line.find(Edge_Three_Noir2) == line.size() - 4)) {
		nb_pion_noir -= 3;
		scores.second += 1000;
	}

   
	if (nb_pion_blanc >= 2) {
		for (const auto& pattern : patterns_blanc_LiveTwo) {
			if (line.find(pattern) != std::string::npos) {
				nb_pion_blanc -= 2;
				scores.first += 5000;
			}
			if (nb_pion_blanc < 2)
				break;
		}
	}

	if (nb_pion_blanc >= 2){
				for (const auto& pattern : patterns_blanc_DeadTwo) {
			if (line.find(pattern) != std::string::npos) {
				nb_pion_blanc -= 2;
				scores.first += 500;
			}
			if (nb_pion_blanc < 2)
				break;
		}
	}
	if (nb_pion_blanc >= 2 && (line.find(Edge_Two_Blanc) == 0) ||
	nb_pion_blanc >= 2 && (line.find(Edge_Two_Blanc2) == line.size() - 3)) {
		nb_pion_blanc -= 2;
		scores.first += 500;
	}

	if (nb_pion_noir >= 2) {
		for (const auto& pattern : patterns_noir_LiveTwo) {
			if (line.find(pattern) != std::string::npos) {
				nb_pion_noir -= 2;
				scores.second += 5000;
			}
			if (nb_pion_noir < 2)
				break;
		}
	}

	if (nb_pion_noir >= 2) {
		for (const auto& pattern : patterns_noir_DeadTwo) {
			if (line.find(pattern) != std::string::npos) {
				nb_pion_noir -= 2;
				scores.second += 500;
			}
			if (nb_pion_noir < 2)
				break;
		}
	}
	if (nb_pion_noir >= 2 && (line.find(Edge_Two_Noir) == 0) ||
		nb_pion_noir >= 2 && (line.find(Edge_Two_Noir2) == line.size() - 3)) {
		nb_pion_noir -= 2;
		scores.second += 500;
	}


    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    // printf("time for ONE score%lld\n",end-begin);
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

int Algo::heuristique(const std::string& map, bool turn, int bscore, int wscore) {
    int score = 0;
    char player = turn ? '2': '1';
    char opponent = turn ? '1': '2';

    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	if (fiveInRow(map, turn, '2'))
		return 1000000;
	if (fiveInRow(map, turn, '1'))
		return -1000000;
	if (bscore == 5)
		return 1000000;
	if (wscore == 5)
		return -1000000;
    std::chrono::steady_clock::time_point begin2 = std::chrono::steady_clock::now();
    auto it = hashedTranspositionTableBoard.find(hasher(map));
    std::chrono::steady_clock::time_point endFind = std::chrono::steady_clock::now();
    timespentToSearchInTranspoTable += std::chrono::duration_cast<std::chrono::milliseconds>(endFind.time_since_epoch()).count() - std::chrono::duration_cast<std::chrono::milliseconds>(begin2.time_since_epoch()).count();
    if (it != hashedTranspositionTableBoard.end()) {
        return it->second;
    }
    for (int i = 0; i < size; i++) {
        std::string row = getRow(map, i);
        auto tmpScores = FindPatternBothPlayers(row);
        score += tmpScores.second - tmpScores.first;

        std::string col = getCol(map, i);
        tmpScores = FindPatternBothPlayers(col);
        score += tmpScores.second - tmpScores.first;

        std::string diagonal = getDiagonalFromRow(map, i);
        tmpScores = FindPatternBothPlayers(diagonal);
        score += tmpScores.second - tmpScores.first;

        diagonal = getDiagonalFromCol(map, i);
        tmpScores = FindPatternBothPlayers(diagonal);
        score += tmpScores.second - tmpScores.first;

    	diagonal = getDiagonalSecondaryFromRow(map, i);
		tmpScores = FindPatternBothPlayers(diagonal);
		score += tmpScores.second - tmpScores.first;

		diagonal = getDiagonalSecondaryFromCol(map, i);
		tmpScores = FindPatternBothPlayers(diagonal);
		score += tmpScores.second - tmpScores.first;
    }
    // transpositionTableBoard[map] = score;
    hashedTranspositionTableBoard[hasher(map)] = score;
	// printf("bscore: %d\n", bscore);
	// printf("wscore: %d\n", wscore);
	// if (bscore >= 0 || wscore >= 0)
	// 	printf("avent score: %d\n", score);
	if (bscore == 4)
		score += 12000;
	if (wscore == 4)
		score -= 12000;
	if (bscore == 3)
		score += 10000;
	if (wscore == 3)
		score -= 10000;
	if (bscore == 2)
		score += 8000;
	if (wscore == 2)
		score -= 8000;
	if (bscore == 1)
		score += 5000;
	if (wscore == 1)
		score -= 5000;
	// if (bscore >= 0 || wscore >= 0)
	// 	printf("apres score: %d\n", score);

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    timespentInHeuristic += std::chrono::duration_cast<std::chrono::milliseconds>(end.time_since_epoch()).count() - std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()).count();
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

	int maxDepth = 3;
    int result = 0;
	int best_result = 0;
	int best_move = 0;
	int best_alpha = INT_MIN;
	int best_beta = INT_MAX;
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    for (int currentDepth = maxDepth; currentDepth <= maxDepth; currentDepth++) {
		iterativeDepth = currentDepth;
		printf("currentDepth: %d\n", currentDepth);
        result = minMax(data.map, INT_MIN, INT_MAX, currentDepth, data.turn, &begin, Map_bScore, Map_wScore);
		if (result == -1){
            printf("minMax failed\n");
			result = best_result;
			optimalMove = best_move;
		}

		// printf("optimalAlpha: %d\n", optimalAlpha);
		// printf("bestalpha: %d\n", best_alpha);
		// printf("optimalBeta: %d\n", optimalBeta);
		// printf("bestbeta: %d\n", best_beta);
		// printf("best_move: %c, %d\n", optimalMove % size + 'A', size - (optimalMove / size));

		if (data.turn == true ){
			if (best_alpha < optimalAlpha && optimalAlpha != INT_MIN) {
				best_result = result;
				best_move = optimalMove;
				best_alpha = optimalAlpha;
				// printf("\n\n");
				// printf("best_move: %c, %d\n", best_move % size + 'A', size - (best_move / size));
				// printf("best_result: %d\n", best_result);
				// printf("optimalAlpha: %d\n", optimalAlpha);
				// printf("optimalBeta: %d\n", optimalBeta);
				// printf("\n\n");
			}
		}
		else if (data.turn == false ){
			if (best_beta > optimalBeta && optimalBeta != INT_MAX) {
				best_beta = optimalBeta;
				best_result = result;
				best_move = optimalMove;
				// printf("\n\n");
				// printf("best_move: %c, %d\n", best_move % size + 'A', size - (best_move / size));
				// printf("best_result: %d\n", best_result);
				// printf("optimalAlpha: %d\n", optimalAlpha);
				// printf("optimalBeta: %d\n", optimalBeta);
				// printf("\n\n");
			}
		}
		
        printf("size of transposition table line %lld\n",transpositionTable_Line.size());
        // printf("size of transposition table board%lld\n",transpositionTableBoard.size());
        printf("size of transposition table board%lld\n",hashedTranspositionTableBoard.size());
		std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - begin).count();
    }

	// TEST A ENLEVER
	if (best_result == 0){
		best_result = result;
		best_move = optimalMove;
	}
	else {
		result = best_result;
		optimalMove = best_move;
	}
	//

	// printf("\n\nTOUR =========================\n");

	// printf("turn: %d\n", data.turn);
	// printf("player: %d\n", player_dark);


	// printf("result: %d\n", result);
	// printf("optimalMove: %d\n", optimalMove);
	// printf("optimalMove: %c, %d\n", optimalMove % size + 'A', size - (optimalMove / size));
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	printf("Time difference (milliseconds) = %lld\n", std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());
	printf("     =========================\n\n");
    printf("Lines checked: %d\n",countCheckedLine);
    printf("Prunes done: %d\n",nbOfPruning);
    printf("timespentInMovesOrder: %lld\n",timespentInSetMovesOrder);
    printf("timespentInHeuristic: %lld\n",timespentInHeuristic);
    printf("timespentInSearchTableLine: %lld\n",timespentToSearchInTranspoTable);
    printf("timespentInSearchTableLine: %lld\n",timespentInSomeFunctions);
	
    // historique.clear();
	// historique.resize(0);
	movesOrder.clear();

	return optimalMove;
}

int Algo::minMax(const std::string& position, int alpha, int beta, int depth, bool turn, std::chrono::steady_clock::time_point* begin, int bScore, int wScore) {
    countCheckedLine++;
    if(depth>3)printf("startedAlgoAtDepth: %d\n",depth);
    // Vérifier le temps écoulé
    // std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    // auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - *begin).count();
    // if (elapsed_ms > 100000) {
    //     printf("toomuch time, aborted on depth%d\n",depth);
    //     // return -1;
	// 	if (fiveInRow(position, turn, '2')) {
	// 		return 10000000;
	// 	}
	// 	else if (fiveInRow(position, turn, '1')) {
	// 		return -10000000;
	// 	}
    //     return heuristique(position, turn, bScore, wScore);
    // }

    if (fiveInRow(position, turn, '2') || bScore == 5) {
        return 10000000;
    }
    else if (fiveInRow(position, turn, '1') || wScore == 5) {
        return -10000000;
    }

    if (depth == 0 || bScore == 5 || wScore == 5){
        return heuristique(position, turn, bScore, wScore); // return current score
    }

    // std::vector<int> moves = setMovesOrder(position, turn);
    std::vector<int> moves = setMovesOrderLineScore(position, turn);

    if (turn) {
        int maxEval = INT_MIN;
        for (int move : moves) {
            std::string updatedMap = position;

            // Modifiez updatedMap pour inclure le mouvement en cours
            updatedMap[move] = '2';



            std::vector<int> captured = getCaptureIndices(updatedMap, move, turn);

            if (!captured.empty()) {
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
					printf("eval: %d\n", eval);
					alpha = std::max(alpha, maxEval);

                    optimalMove = move;
                    optimalAlpha = alpha;
                    optimalBeta = beta;
                }
            }
            if (beta <= alpha){
                if(depth>3)printf("prune done at depth%d\n",depth);
                nbOfPruning++;
                break;
            }
        }
        if(depth == iterativeDepth) printf("minMax Ended\n");
        return maxEval;
    }
    else {
        int minEval = INT_MAX;
        for (int move : moves) {
            std::string updatedMap = position;

            // Modifiez updatedMap pour inclure le mouvement en cours
            updatedMap[move] = '1';

            std::vector<int> captured = getCaptureIndices(updatedMap, move, turn);
            if (!captured.empty()) {
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
                if(depth>3)printf("prune done at depth%d\n",depth);
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
    vector<int> result;
    char player = turn ? '2': '1';
    char opponent = turn ? '1': '2';
    orderedMoves.clear();
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            if (map[y * size + x] != '0') {
                for (int dy = -2; dy <= 2; ++dy) {
                    for (int dx = -2; dx <= 2; ++dx) {
                        int newX = x + dx;
                        int newY = y + dy;
                        if (newX < 0 || newX >= size || newY < 0 || newY >= size || map[newY * size + newX] != '0') {
                            continue;
                        }
                        // unorderedMoves.push_back(pair<int,int>(10,newY * size + newX));
                        // orderedMoves.push_back(pair<int,int>(checkScorePos(map,newY,newX,turn),newY * size + newX));
                        result.push_back(newY * size + newX);
                    }
                }
            }
        }
    }
    //to add to order vector
    // sort(orderedMoves.begin(), orderedMoves.end(),greater<pair<int, int> >());
    // for(auto& pair : orderedMoves){
    //     result.push_back(pair.second);
    // }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    timespentInSetMovesOrder += std::chrono::duration_cast<std::chrono::milliseconds>(end.time_since_epoch()).count() - std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()).count();
    return result;
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
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
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
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    timespentInSetMovesOrder += std::chrono::duration_cast<std::chrono::milliseconds>(end.time_since_epoch()).count() - std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()).count();
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