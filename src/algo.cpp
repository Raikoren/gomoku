#include "algo.hpp"

int Algo::algo(char map[362], bool turn) {
    minMax(map, std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), DEPTH, turn);
}

void Algo::minMax(int position, float alpha, float beta, int depth, bool turn) {
    if (depth == 0 || bScore == 5 || wScore == 5)
        return; // return current score
    if (turn) {
        int maxEval = -std::numeric_limits<float>::infinity();
        int child = 0;
        while (newPosition(child, position)) {
            int eval = minMax(position + 1, alpha, beta, depth - 1, !turn);
            //maxEval = heuristique(maxEval, eval);
            //alpha = heuristique(alpha, eval);
            if (beta <= alpha) {
                break;
            }
        }
        return maxEval;
    }
    else {
        int minEval = std::numeric_limits<float>::infinity();
        int child = 0;
        while (newPosition(child, position)) {
            int eval = minMax(position + 1, alpha, beta, depth - 1, !turn);
            //minEval = heuristique(minEval, eval);
            //alpha = heuristique(alpha, eval) * -1;
            if (beta <= alpha) {
                break;
            }
        }
        return minEval;
    }
}

/*
** fonction qui va faire la nouvelle position.
** child = le nombre de fois qu'on doit avancer avec l'algo qui selectione le coup a jouer
** l'algo devra partir d'un endroit du goban ou les pions sont deja la et partir en escargot pour selectionner un tour qui a un minimum de sens
** on pourrait aussi prioriser les coup qui creent des double ligne triple, des lignes triple ou qui provoque un mangeage
** position = position dans l'historique
** centerX/Y = le centre de la ou tout les pion sont places
*/
bool Algo::newPosition(int child, int position, int centerX, int centerY, bool turn) {
    int i = 0;
    int y = centerY + 1;
    int x = centerX;
    while (i <= child) { // pour optimiser, c'est ici qu'on peut prioriser avec une premiere boucle les coup important
        x += 1; //condition pour faire +1 +0 ou -1 pour escargot
        y += 1; //condition pour faire +1 +0 ou -1 pour escargot
        i += 0; //condition pour faire +1 ou +0 selon si le coup est valide 
    } // on sort de la avec un couple x y qui represente la positionnement qu'on va tester
    // creation d'une map avec le coup dans le vecteur, en utilisant position
}