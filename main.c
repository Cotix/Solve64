#include "transpositions.h"
#include "board.h"
#include <stdlib.h>
#include <stdio.h>
#define INFINITY 100000
#define FULLSEARCH 1
unsigned long long workCounter = 0;
unsigned long long realWork = 0;
int order[] = {5,6,9,10,0,3,12,15,8,4,1,2,7,11,14,13};

int alphabeta(int depth, int alpha, int beta, int color) {
    int transType = getType();
    if (transType != TT_EMPTY) { 
        if (transType == TT_EXACT) {
            workCounter += (1<<getWork());
            return getScore();
        }
        else if (transType == TT_UPPER) {
            beta = 0;
            if (alpha >= beta) { 
                workCounter += (1<<getWork());
                return getScore();
            }
        } else if (transType == TT_LOWER) {
            alpha = 0;
            if (alpha >= beta) {
                workCounter += (1<<getWork());
                return getScore();
            }
        }        
    }
    unsigned long long oldWork = workCounter++;
    int oldAlpha = alpha;
    if (hasWon(!color)) return -INFINITY;
    if (depth == 0 || isFull()) return 0;

    for (int idx = 0; idx != 16; ++idx) {
        if (!move(idx, color)) continue;
        if (hasWon(color)) {
            unmove(idx, color);
            return INFINITY;
        }
        unmove(idx, color);
        if (!move(idx, !color)) continue;
        if (hasWon(!color)) {
            unmove(idx,!color);
            move(idx, color);
            int res = -alphabeta(depth-1, -beta, -alpha, !color);
            unmove(idx, color);
            return res;
        }
        unmove(idx, !color);
    }

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
    if (best != 0 || FULLSEARCH != 0) putTable(best, logWork, f);
    realWork++;
    return best;
}




int main(int argc, char **argv) {
    initZobrist();
    initTable(120001099);
    initBoard();
    printf("%i\n", alphabeta(atoi(argv[1]), -INFINITY, INFINITY, 0));
    printf("misses:%llu workcounter:%llu realwork:%llu\n", transMisses, workCounter, realWork); 

    return 0;
}
