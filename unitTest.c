#include "unitTestBoard.h"
#include "board.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
void testHorWon();
void clear();

void clear() {
    board[0] = 0L; board[1] = 0L;
}


void test2dWon(){
    clear();
    puts("Testing 2d win check");
    int i;
    for (i = 0; i != winningElements; ++i) {
        board[0] = winningBoards[i];
        printf("Testing for win %i %llu\n", i, board[0]);
        fflush(stdout);
        assert(hasWon(0) == 1);
    }
    for (i = 0; i != losingElements; ++i) {
        board[0] = losingBoards[i];
        printf("Testing for lose %i %llu\n", i, board[0]);
        fflush(stdout);
        assert(hasWon(0) == 0);
    }
}



int main() {
   test2dWon(); 
}
