#ifndef ALGO_HPP
# define ALGO_HPP

# include <iostream>
# include <string.h>
# include <algorithm>
#include <chrono>
#include <vector>
#include <utility>
#include <tgmath.h>
#include <unordered_map>
#include "env.hpp"
#include <functional>
#include <regex>
#include <set>
#include <map>


# define Eat_Blanc			"2110"
# define Eat_Blanc_1		"0112"

# define Eat_Noir			"1220"
# define Eat_Noir_1			"0221"

# define FiveInRow_Blanc	"11111"

// four

# define LiveFour_Blanc		"011110"
# define LiveFour_Blanc_1  "0101110"
# define LiveFour_Blanc_2  "0111010"

# define DeadFour_Blanc		"211110"
# define DeadFour_Blanc_1  "2101110"
# define DeadFour_Blanc_2  "0111102"
# define DeadFour_Blanc1_1	"011112"
# define DeadFour_Blanc2	"11101"
# define DeadFour_Blanc2_2	"10111"
# define DeadFour_Blanc3	"11011"

# define LiveFour_Noir		"022220"
# define LiveFour_Noir_1   "0202220"
# define LiveFour_Noir_2   "0222020"

# define DeadFour_Noir		"122220"
# define DeadFour_Noir_1   "1202220"
# define DeadFour_Noir_2   "0222201"
# define DeadFour_Noir1_1	"022221"
# define DeadFour_Noir2		"22202"
# define DeadFour_Noir2_2	"20222"
# define DeadFour_Noir3		"22022"

// three

# define LiveThree_Blanc	"01110"
# define LiveThree_Blanc_1 "010110"
# define LiveThree_Blanc_2 "011010"

# define DeadThree_Blanc	"21110"
# define DeadThree_Blanc_1 "210110"
# define DeadThree_Blanc_2 "011012"
# define DeadThree_Blanc1_1	"001112"
# define DeadThree_Blanc2	"1101"
# define DeadThree_Blanc2_2	"1011"
# define DeadThree_Blanc3	"011001"
# define DeadThree_Blanc3_2	"100110"
# define DeadThree_Blanc4	"10101"

# define LiveThree_Noir		"02220"
# define LiveThree_Noir_1  "020220"
# define LiveThree_Noir_2  "022020"

# define DeadThree_Noir		"12220"
# define DeadThree_Noir_1   "120220"
# define DeadThree_Noir_2   "022021"
# define DeadThree_Noir1_1	"002221"
# define DeadThree_Noir2	"2202"
# define DeadThree_Noir2_2	"2022"
# define DeadThree_Noir3	"022002"
# define DeadThree_Noir3_2	"200220"
# define DeadThree_Noir4	"20202"

// two


# define LiveTwo_Blanc		"10001"
# define LiveTwo_Blanc2		"10010"
# define LiveTwo_Blanc2_2	"01001"
# define LiveTwo_Blanc3		"01010"
# define LiveTwo_Blanc4		"001100"

# define DeadTwo_Blanc		"211000"
# define DeadTwo_Blanc1_1	"000112"
# define DeadTwo_Blanc2		"210100"
# define DeadTwo_Blanc2_2	"001012"
# define DeadTwo_Blanc3		"210010"

# define LiveTwo_Noir		"20002"
# define LiveTwo_Noir2		"20020"
# define LiveTwo_Noir2_2	"02002"
# define LiveTwo_Noir3		"02020"
# define LiveTwo_Noir4		"002200"

# define DeadTwo_Noir		"122000"
# define DeadTwo_Noir1_1	"000221"
# define DeadTwo_Noir2		"120200"
# define DeadTwo_Noir2_2	"002021"
# define DeadTwo_Noir3		"120020"


// edge	

# define Edge_Four_Blanc	"11110"
# define Edge_Three_Blanc	"1110"
# define Edge_Two_Blanc		"110"
# define Edge_Four_Blanc2	"01111"
# define Edge_Three_Blanc2	"0111"
# define Edge_Two_Blanc2	"011"




# define FiveInRow_Noir		"22222"


# define Edge_Four_Noir		"22220"
# define Edge_Three_Noir	"2220"
# define Edge_Two_Noir		"220"
# define Edge_Four_Noir2	"02222"
# define Edge_Three_Noir2	"0222"
# define Edge_Two_Noir2		"022"

using namespace std;

struct AlgoData {
    int     wScore;// une fois a 5 l'algo ne doit pas aller plus loin
    int     bScore;// une fois a 5 l'algo ne doit pas aller plus loin
    char    map[361];// map original
    bool    turn;// true = black || false == white
    int     size;
};

class Algo {
	public:
		Algo() {
			patterns_blanc_DeadFour = {DeadFour_Blanc, DeadFour_Blanc_1, DeadFour_Blanc_2,DeadFour_Blanc1_1, DeadFour_Blanc2, DeadFour_Blanc2_2, DeadFour_Blanc3};
			patterns_blanc_LiveFour = {LiveFour_Blanc, LiveFour_Blanc_1, LiveFour_Blanc_2};

			patterns_blanc_DeadThree = {DeadThree_Blanc, DeadThree_Blanc_1, DeadThree_Blanc_2 ,DeadThree_Blanc1_1, DeadThree_Blanc2, DeadThree_Blanc2_2, DeadThree_Blanc3, DeadThree_Blanc3_2, DeadThree_Blanc4};
			patterns_blanc_LiveThree = {LiveThree_Blanc, LiveThree_Blanc_1, LiveThree_Blanc_2};

			patterns_blanc_LiveTwo = {LiveTwo_Blanc, LiveTwo_Blanc2, LiveTwo_Blanc2_2, LiveTwo_Blanc3, LiveTwo_Blanc4};
			patterns_blanc_DeadTwo = {DeadTwo_Blanc, DeadTwo_Blanc1_1, DeadTwo_Blanc2, DeadTwo_Blanc2_2, DeadTwo_Blanc3};

			patterns_noir_DeadFour = {DeadFour_Noir, DeadFour_Blanc_1, DeadFour_Blanc_2,DeadFour_Noir1_1, DeadFour_Noir2, DeadFour_Noir2_2, DeadFour_Noir3};
			patterns_noir_LiveFour = {LiveFour_Noir, LiveFour_Noir_1, LiveFour_Noir_2};

			patterns_noir_DeadThree = {DeadThree_Noir, DeadThree_Noir_1, DeadThree_Noir_2,DeadThree_Noir1_1, DeadThree_Noir2, DeadThree_Noir2_2, DeadThree_Noir3, DeadThree_Noir3_2, DeadThree_Noir4};
			patterns_noir_LiveThree = {LiveThree_Noir, LiveThree_Noir_1, LiveThree_Noir_2};

			patterns_noir_LiveTwo = {LiveTwo_Noir, LiveTwo_Noir2, LiveTwo_Noir2_2, LiveTwo_Noir3, LiveTwo_Noir4};
			patterns_noir_DeadTwo = {DeadTwo_Noir, DeadTwo_Noir1_1, DeadTwo_Noir2, DeadTwo_Noir2_2, DeadTwo_Noir3};

			patterns_eatnoir = {Eat_Noir, Eat_Noir_1};
			patterns_eatblanc = {Eat_Blanc, Eat_Blanc_1};
		}
        int     ask(AlgoData data); // fonction d'appel
		int minMax(const std::string& position, int alpha, int beta, int depth, bool turn, std::chrono::steady_clock::time_point* begin, int bScore, int wScore);
        // position = position dans l'historique (0 <-> DEPTH)
        // alpha = meilleur score actuel pour les blancs
        // beta = meilleur score actuel pour les noirs
        // depth = profondeur actuelle
        std::vector<int> setMovesOrder(const std::string& i, bool turn); // retourne un vecteur contenent dans l'ordre les coups � tester 
		std::vector<std::pair<int, int>> getWindowBounds(const std::string& map);
        bool    checkPos(int x, int y, std::string map, bool turn); // check valibilit� d'un coup et si firstRound == true, si le coup cr�� une ligne ou une prise
		std::vector<int> setMovesOrderLineScore(const std::string& i, bool turn); // retourne un vecteur contenent dans l'ordre les coups à tester
        int     checkScorePos(string mapWithIncomingNewMove, int newY, int newX, bool turn);
        int		heuristique(const std::string& map, bool turn, int bscore, int wscore);
        // threeLine() detecte si le placement genere une ligne de trois
        bool    threeLine(int dx, int dy, int x, int y, const std::string map, bool turn);
        // antiline() detecte si le coup contre une ligne de trois déjà presente (take advantage est la meme fonction je pense faut quej e vois)
        bool    antiLine();
        // takeAdvantage() detecte si le coup génère un paterne qui provoque un puissance 5 dans deux tour à l'aide d'une ligne de 3
        bool    takeAdvantage(int dx, int dy, int x, int y, std::string map, bool turn);
		bool	fourLine(int dx, int dy, int x, int y, const std::string map, bool turn);
		int		calculateScoreRow(const std::string& map, char player);
        bool    canTake(int x, int y, std::string map, bool turn);
		bool	fiveInRow(const std::string& map, bool turn, char player);

		int		FindPattern(const std::string line, char player);
		std::vector<int> getCaptureIndices(const std::string& map, int move, bool turn);
		bool isValidCoordinate(int x, int y);

		std::pair<int, int> FindPatternBothPlayers(const std::string &line);

		std::string	getRow(const std::string& map, int row);
		std::string getCol(const std::string& map, int col);
		std::string getDiagonalFromRow(const std::string& map, int row);
		std::string getDiagonalFromCol(const std::string& map, int col);
		std::string getDiagonalSecondaryFromRow(const std::string& map, int row);
		std::string getDiagonalSecondaryFromCol(const std::string& map, int col);

		bool checkGoodPos(const std::string& mapWithIncomingNewMove, int newY, int newX, bool turn);


	private:

		struct pair_hash {
			template <class T1, class T2>
			std::size_t operator () (const std::pair<T1, T2> &pair) const {
				std::size_t h1 = std::hash<T1>()(pair.first);
				std::size_t h2 = std::hash<T2>()(pair.second);
				return h1 ^ h2;
			}
		};

		bool						eat_pion_blanc; //test
		bool						eat_pion_noir; //test

		int							iterativeDepth;
		int							optimalMove;
		int							optimalAlpha;
		int							optimalBeta;
        int                         Map_bScore;
        int                         Map_wScore;
        int                         size;
		bool						player_dark;
        int                         countCheckedLine;
        int                         nbOfPruning;
        long                        timespentInSetMovesOrder;
        long                        timespentInHeuristic;
        long                        timespentToSearchInTranspoTable;
        long                        timespentInSomeFunctions;
		std::hash<std::string> 		hasher;
        std::vector<pair<int, int>> orderedMoves;  // <score/position>
        std::vector<std::string>    historique; //historique de longueur DEPTH
        std::vector<std::string>    movesOrder; //historique de longueur DEPTH
		std::unordered_map<std::string, int> transpositionTableBoard; // map/score
		std::unordered_map<int, int> hashedTranspositionTableBoard; // map/score
		std::unordered_map<std::string, pair<int, int>> transpositionTable_Line;


		std::vector<std::string> patterns_blanc_DeadFour;
		std::vector<std::string> patterns_blanc_LiveFour;
		std::vector<std::string> patterns_blanc_DeadThree;
		std::vector<std::string> patterns_blanc_LiveThree;
		std::vector<std::string> patterns_blanc_LiveTwo;
		std::vector<std::string> patterns_blanc_DeadTwo;

		std::vector<std::string> patterns_noir_DeadFour;
		std::vector<std::string> patterns_noir_LiveFour;
		std::vector<std::string> patterns_noir_DeadThree;
		std::vector<std::string> patterns_noir_LiveThree;
		std::vector<std::string> patterns_noir_LiveTwo;
		std::vector<std::string> patterns_noir_DeadTwo;

		std::vector<std::string> patterns_eatnoir;
		std::vector<std::string> patterns_eatblanc;

		// std::unordered_map<std::pair<std::string, char>, int, pair_hash> transpositionTable_Line;

};

#endif