#include "settings.h"
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
    return (~((transBoardPotential >>64)*19L)^transBoardPotential)^board[0]^board[1];
}

void initBoard() {
    memset(height, 0, sizeof(unsigned int)*16);
    hash = 0;
    board[0] = 0;
    board[1] = 0;
    myColor = 0;
    moves = 0;
}

inline __uint128_t rotate(__uint128_t b) {
    int rotation[] = {3, 6, 9, 12, -2, 1, 4, 7, -7, -4, -1, 2, -12, -9, -6, -3};
    __uint128_t mask = 0;
    __uint128_t result = 0;
    for (int i = 0; i != 16; ++i) mask = (mask<<16)|1;
    for (int i = 0; i != 16; ++i) {
        if (rotation[i] > 0) {
            result |= (b&mask)<<rotation[i];
        } else {
            result |= (b&mask)>>(-rotation[i]);
        }
        mask <<= 1;
    }
    return result;
}

inline __uint128_t getBoard(int full) {
    unsigned long long b0 = board[0];
    unsigned long long b1 = board[1];
    if (full) {
        b0 &= getPotential(0);
        b1 &= getPotential(1);
    }
    __uint128_t b = (((__uint128_t) b0)<<64) | ((__uint128_t) b1);
    if (moves > CALCULATE_ROTATIONS_UPTO) return b;
    __uint128_t biggest = b;
    for (int i = 0; i != 4; ++i) {
        b = rotate(b);
        if (b > biggest) {
            biggest = b;
        }
    }
    return biggest;
}

inline int move(int pos, int player) {
    if (height[pos] == 4) return 0;
    hash ^= zobrist[(height[pos]*16 + pos) + 64*player];
    board[player] |= 1L << (height[pos]*16 + pos);
    height[pos]++;
    moves++;
    transBoard = getBoard(0);
    transBoardPotential = getBoard(ONLY_CONSIDER_POTENT);
    return 1;
}

inline void unmove(int pos, int player) {
    moves--;
    board[player] ^= 1L << ((--height[pos])*16 + pos);
    hash ^= zobrist[(height[pos]*16 + pos) + 64*player];
    transBoard = getBoard(0);
    transBoardPotential = getBoard(ONLY_CONSIDER_POTENT);
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
