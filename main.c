#include "transpositions.h"
#include "board.h"
#include <stdlib.h>
#include <stdio.h>
#define INFINITY 100000

unsigned long long workCounter = 0;
int order[] = {5,6,9,10,12,15,3,0,8,4,1,2,7,11,14,13};

int alphabeta(int depth, int alpha, int beta, int color) {
    unsigned long long oldWork = workCounter++;
    int transType = getType();
    if (transType != TT_EMPTY) { 
        if (transType == TT_EXACT) return getScore();
        else if (transType == TT_UPPER) {
            beta = 0;
            if (alpha >= beta) return getScore();
        } else if (transType == TT_LOWER) {
            alpha = 0;
            if (alpha >= beta) return getScore();
        }        
    }
    int oldAlpha = alpha;
    if (hasWon(!color)) return -INFINITY;
    if (depth == 0 || isFull()) return 0;
    int best = -INFINITY;
    for (int idx = 0; idx != 16; ++idx) {
        int i = order[idx];
        if (!move(i, color)) continue;
        int res = -alphabeta(depth-1, -beta, -alpha, !color);
        unmove(i, color);
        best = res > best ? res : best;
        alpha = res > alpha ? res : alpha;
        if (alpha >= beta) break;
    }
    int f = TT_EXACT;
    if (best <= oldAlpha) f = TT_UPPER;
    else if (best >= beta) f = TT_LOWER;
    unsigned long long work = workCounter - oldWork;
    unsigned long long logWork = 0;
    while ((work>>=1) > 0) logWork++;
    putTable(best, logWork, f);
    return best;
}




int main() {
    initBoard();
    initTable(167772161);
    printf("%i\n", alphabeta(13, -INFINITY, INFINITY, 0));
}
