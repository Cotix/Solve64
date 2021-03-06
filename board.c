#include "board.h"
#include "util.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>

void initZobrist() {
    srand(0x51c9f6e6);
    for (int i = 0; i != 128; ++i)
        zobrist[i] = rand64();
    srand(time(0));
}

inline unsigned long long getHash() {
    unsigned long long p1 = getPotential(0);
    unsigned long long p2 = getPotential(1);
    return (((board[0]&p1)*2L) ^ ((board[1]&p1))) * (board[0]|board[1]);
}

void initBoard() {
    memset(height, 0, sizeof(unsigned int)*16);
    hash = 0;
    board[0] = 0;
    board[1] = 0;
    myColor = 0;
}

inline int move(int pos, int player) {
    if (height[pos] == 4) return 0;
    hash ^= zobrist[(height[pos]*16 + pos) + 64*player];
    board[player] |= 1L << (height[pos]*16 + pos);
    height[pos]++;
    return 1;
}

inline void unmove(int pos, int player) {
    board[player] ^= 1L << ((--height[pos])*16 + pos);
    hash ^= zobrist[(height[pos]*16 + pos) + 64*player];
}

inline int isFull() {
    return (board[0]|board[1]) == BOARD_FULL;
}

inline int winCheck(int player, unsigned long long mask, int shift) {
    unsigned long long h = board[player];
    int i;
    for (i = 0; i != 3; ++i)
        h = ((h&(~mask))<<shift)&board[player];
    if (h != 0) return 1;
    return 0;
}

inline unsigned long long potentialCheck(int player, unsigned long long mask, int shift) {
    unsigned long long h = BOARD_FULL;
    mask |= board[!player];
    int i;
    for (i = 0; i != 3; ++i)
        h = ((h&(~mask))<<shift);
    for (i = 0; i != 3; ++i)
        h |= ((h&(~mask))>>shift);

    return h;
}

inline unsigned long long getPotential(int player) {
    //First check inside of the horizontal fields
    //Check for horizontal 4 in a row
    return BOARD_FULL;
    unsigned long long result = 0;
    result |= potentialCheck(player, FIELD_HOR_EDGES, 1);
    result |= potentialCheck(player, FIELD_VER_EDGES, 4);
    result |= potentialCheck(player, FIELD_DIA_TLEFT, 5);
    result |= potentialCheck(player, FIELD_DIA_BLEFT, 3);
    result |= potentialCheck(player, 0L, 16);
    result |= potentialCheck(player, FIELD_VER_EDGES, 20);
    result |= potentialCheck(player, FIELD_DIA_3D_RIGHT, 17);
    result |= potentialCheck(player, FIELD_DIA_3D_DOWN, 12);
    result |= potentialCheck(player, FIELD_DIA_3D_LEFT, 15);    

    if (((board[player]|(~board[!player]))&FIELD_DIA1) == FIELD_DIA1)
        result |= FIELD_DIA1;
    if (((board[player]|(~board[!player]))&FIELD_DIA1) == FIELD_DIA2)
        result |= FIELD_DIA2;
    if (((board[player]|(~board[!player]))&FIELD_DIA1) == FIELD_DIA3)
        result |= FIELD_DIA3;
    if (((board[player]|(~board[!player]))&FIELD_DIA1) == FIELD_DIA4)
        result |= FIELD_DIA4;

    return result;
}


inline int hasWon(int player) {
    //First check inside of the horizontal fields
    //Check for horizontal 4 in a row
    if (winCheck(player, FIELD_HOR_EDGES, 1) != 0) return 1;
    //Check for vertical 4 in a row
    if (winCheck(player, FIELD_VER_EDGES, 4) != 0) return 1;
    //Check diagonal topleft to bottom right
    if (winCheck(player, FIELD_DIA_TLEFT, 5) != 0) return 1;
    //Check diagonal topright to bottom left
    if (winCheck(player, FIELD_DIA_BLEFT, 3) != 0) return 1;

    //Now lets do the 3d checks.
    //First vertical stacks.
    if (winCheck(player, 0L, 16) != 0) return 1;
    //Now the 4 diagonals
    if (winCheck(player, FIELD_VER_EDGES, 20) != 0) return 1;
    if (winCheck(player, FIELD_DIA_3D_RIGHT, 17) != 0) return 1;
    if (winCheck(player, FIELD_DIA_3D_DOWN, 12) != 0) return 1;
    if (winCheck(player, FIELD_DIA_3D_LEFT, 15) != 0) return 1;
    
    if ((board[player]&FIELD_DIA1) == FIELD_DIA1) return 1;
    if ((board[player]&FIELD_DIA2) == FIELD_DIA2) return 1;
    if ((board[player]&FIELD_DIA3) == FIELD_DIA3) return 1;
    if ((board[player]&FIELD_DIA4) == FIELD_DIA4) return 1;

    return 0;
}
