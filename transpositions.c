#include "board.h"
#include "transpositions.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void initTable(unsigned long long size) {
    transSize = size;
    table = (struct bucket*)malloc(sizeof(struct bucket)*size);
    if (table == 0) {
        perror("Can not allocate transtable");
    }
    memset(table, 0, sizeof(struct bucket)*size);
}

inline void save(int score, int work, int flag, int s, int idx) {
    table[idx].slot[s].board[0] = board[0]&getPotential(0);
    table[idx].slot[s].board[1] = board[1]&getPotential(1);
    table[idx].slot[s].board[2] = board[1]|board[0];
    table[idx].slot[s].data = flag | (score<<2);
    table[idx].slot[s].work = work;
}

inline void putTable(int score, int work, int flag) {
    unsigned long long idx = getHash()%transSize;
    if (table[idx].slot[1].work <= work) {
        save(score, work, flag, 1, idx);
    } else {
        save(score, work, flag, 0, idx);
    }
}

inline int getType() {
    unsigned long long idx = getHash()%transSize;
    unsigned long long p1 = board[0]&getPotential(0);
    unsigned long long p2 = board[1]&getPotential(1);
    if (table[idx].slot[0].board[0] == p1 &&
        table[idx].slot[0].board[1] == p2 &&
        table[idx].slot[0].board[2] == board[0]|board[1]) {
        return table[idx].slot[0].data&TYPE_MASK;
    }
    else if (table[idx].slot[1].board[0] == p1 &&
             table[idx].slot[1].board[1] == p2 &&
             table[idx].slot[1].board[2] == board[0]|board[1]) {
        return table[idx].slot[1].data&TYPE_MASK;
    }
    return TT_EMPTY;
}

inline int getScore() {
    unsigned long long idx = getHash()%transSize;
    unsigned long long p1 = board[0]&getPotential(0);
    unsigned long long p2 = board[1]&getPotential(1);
    if (table[idx].slot[0].board[0] == p1 &&
        table[idx].slot[0].board[1] == p2 &&
        table[idx].slot[0].board[2] == board[0]|board[1]) {
        return table[idx].slot[0].data&RESULT_MASK;
    }
    else if (table[idx].slot[1].board[0] == p1 &&
             table[idx].slot[1].board[1] == p2 &&
             table[idx].slot[1].board[2] == board[0]|board[1]) {
        return table[idx].slot[1].data&RESULT_MASK;
    }
    return 0;
}
