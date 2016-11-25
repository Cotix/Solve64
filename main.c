#include "board.h"
#include <stdlib.h>
#include <stdio.h>
#define INFINITY 100000


int alphabeta(int depth, int alpha, int beta, int color) {
    if (hasWon(!color)) return -INFINITY;
    if (depth == 0 || isFull()) return 0;
    int best = -INFINITY;
    for (int i = 0; i != 16; ++i) {
        if (!move(i, color)) continue;
        int res = -alphabeta(depth-1, -beta, -alpha, !color);
        unmove(i, color);
        best = res > best ? res : best;
        alpha = res > alpha ? res : alpha;
        if (alpha >= beta) break;
    }

    return best;
}




int main() {
    board[0] = 0; board[1] = 0;
    printf("%i\n", alphabeta(10, -INFINITY, INFINITY, 0));
}
