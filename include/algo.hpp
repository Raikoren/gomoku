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

# define Eat_Blanc			"2112"
# define Eat_Blanc_1		"2112"

# define Eat_Noir			"1221"
# define Eat_Noir_1			"1221"

# define FiveInRow_Blanc	"11111"


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


# define LiveThree_Blanc	"01110"
# define LiveThree_Blanc_1 "010110"
# define LiveThree_Blanc_2 "011010"

# define DeadThree_Blanc	"21110"
# define DeadThree_Blanc_1 "210110"
# define DeadThree_Blanc_2 "011012"
# define DeadThree_Blanc1_1	"01112"
# define DeadThree_Blanc2	"1101"
# define DeadThree_Blanc2_2	"1011"
# define DeadThree_Blanc3	"011001"
# define DeadThree_Blanc3_2	"100110"
# define DeadThree_Blanc4	"10101"


# define LiveTwo_Blanc		"10001"
# define LiveTwo_Blanc2		"10010"
# define LiveTwo_Blanc2_2	"01001"
# define LiveTwo_Blanc3		"01010"
# define LiveTwo_Blanc4		"0110"

# define DeadTwo_Blanc		"2110"
# define DeadTwo_Blanc1_1	"0112"
# define DeadTwo_Blanc2		"2101"
# define DeadTwo_Blanc2_2	"1012"
# define DeadTwo_Blanc3		"21001"


# define Edge_Four_Blanc	"11110"
# define Edge_Three_Blanc	"1110"
# define Edge_Two_Blanc		"110"
# define Edge_Four_Blanc2	"01111"
# define Edge_Three_Blanc2	"0111"
# define Edge_Two_Blanc2	"011"




# define FiveInRow_Noir		"22222"


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


# define LiveThree_Noir		"02220"
# define LiveThree_Noir_1  "020220"
# define LiveThree_Noir_2  "022020"

# define DeadThree_Noir		"12220"
# define DeadThree_Noir_1   "022021"
# define DeadThree_Noir_2   "22012"
# define DeadThree_Noir1_1	"02221"
# define DeadThree_Noir2	"2202"
# define DeadThree_Noir2_2	"2022"
# define DeadThree_Noir3	"022002"
# define DeadThree_Noir3_2	"200220"
# define DeadThree_Noir4	"20202"


# define LiveTwo_Noir		"20002"
# define LiveTwo_Noir2		"20020"
# define LiveTwo_Noir2_2	"02002"
# define LiveTwo_Noir3		"02020"
# define LiveTwo_Noir4		"0220"

# define DeadTwo_Noir		"1220"
# define DeadTwo_Noir1_1	"0221"
# define DeadTwo_Noir2		"1202"
# define DeadTwo_Noir2_2	"2021"
# define DeadTwo_Noir3		"12002"

# define Edge_Four_Noir		"22220"
# define Edge_Three_Noir	"2220"
# define Edge_Two_Noir		"220"
# define Edge_Four_Noir2	"02222"
# define Edge_Three_Noir2	"0222"
# define Edge_Two_Noir2		"022"


struct TranspositionTableEntry {
    std::string map;
    int score;
};


struct AlgoData {
    int     wScore;// une fois a 5 l'algo ne doit pas aller plus loin
    int     bScore;// une fois a 5 l'algo ne doit pas aller plus loin
    char    map[361];// map original
    bool    turn;// true = black || false == white
    int     size;
};

class Algo {
	public:
		Algo() { }
        int     ask(AlgoData data); // fonction d'appel
		int minMax(const std::string& position, int alpha, int beta, int depth, bool turn, std::chrono::steady_clock::time_point* begin, int bScore, int wScore);
        // position = position dans l'historique (0 <-> DEPTH)
        // alpha = meilleur score actuel pour les blancs
        // beta = meilleur score actuel pour les noirs
        // depth = profondeur actuelle
        std::vector<int> setMovesOrder(const std::string& i, bool turn); // retourne un vecteur contenent dans l'ordre les coups � tester 
		std::vector<std::pair<int, int>> getWindowBounds(const std::string& map);
        bool    checkPos(int x, int y, std::string map, bool firstRound, bool turn); // check valibilit� d'un coup et si firstRound == true, si le coup cr�� une ligne ou une prise
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


	private:

		struct pair_hash {
			template <class T1, class T2>
			std::size_t operator () (const std::pair<T1, T2> &pair) const {
				std::size_t h1 = std::hash<T1>()(pair.first);
				std::size_t h2 = std::hash<T2>()(pair.second);
				return h1 ^ h2;
			}
		};

		int							iterativeDepth;
		int							optimalMove;
		int							optimalAlpha;
		int							optimalBeta;
        int                         Map_bScore;
        int                         Map_wScore;
        int                         size;
		bool						player_dark;
        std::vector<std::string>    historique; //historique de longueur DEPTH
        std::vector<std::string>    movesOrder; //historique de longueur DEPTH
		std::unordered_map<std::string, TranspositionTableEntry> transpositionTable;
		std::unordered_map<std::string, std::__1::pair<int, int>> transpositionTable_Line;

		// std::unordered_map<std::pair<std::string, char>, int, pair_hash> transpositionTable_Line;

};

#endif