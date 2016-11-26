#ifndef TRANS_HEADER
#define TRANS_HEADER

#define TYPE_MASK 0x3
#define RESULT_MASK 0xC
#define TT_EMPTY 0x0
#define TT_EXACT 0x1
#define TT_LOWER 0x2
#define TT_UPPER 0x3
#define DRAW 0x0
#define LOSS 0x4
#define WIN 0x8



struct entry {
    unsigned long long board[2];
    unsigned char data;
    unsigned char work;
};
struct bucket {
    struct entry slot[2];
};

unsigned long long transSize;
struct bucket* table;


void initTable(unsigned long long size);
void putTable(int score, int work, int flag);
int getType();
int getScore();
#endif
