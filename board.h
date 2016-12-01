#ifndef BOARD_HEADER
#define BOARD_HEADER


#define FIELD_HOR_EDGES 0b1000100010001000100010001000100010001000100010001000100010001000uL 
#define FIELD_VER_EDGES     0xF000F000F000F000uL 
#define FIELD_DIA_TLEFT     0x0000024802480248uL
#define FIELD_DIA_BLEFT     0x0000042104210421uL
#define FIELD_DIA_3D_LEFT   0x1111111111111111uL 
#define FIELD_DIA_3D_RIGHT  0x8888888888888888uL
#define FIELD_DIA_3D_DOWN   0x000F000F000F000FuL 

#define FIELD_DIA1 9223376434903384065uL
//  FIELD_DIA1 0b1000000000000000000001000000000000000000001000000000000000000001uL
#define FIELD_DIA2 1152923703634296840uL
//  FIELD_DIA2 0b0001000000000000000000100000000000000000010000000000000000001000uL
#define FIELD_DIA3 281612482805760uL
//  FIELD_DIA3 0b0000000000000001000000000010000000000100000000001000000000000000uL
#define FIELD_DIA4 2252074725150720uL
//  FIELD_DIA4 0b0000000000001000000000000100000000000010000000000001000000000000uL


#define BOARD_FULL          0xFFFFFFFFFFFFFFFFuL

//Board is encoded as 64 bits. Every bit is one field.
//the board consists of 4 fields stacked on top that are 4*4.
//Bottom field:
//C  D  E  F
//8  9  A  B
//4  5  6  7
//0  1  2  3
//The fields are stacked ontop of each other.

unsigned long long board[2];
unsigned int height[16];
unsigned int myColor;
unsigned long long hash;
unsigned long long zobrist[128];

void initBoard();
void initZobrist();
int move(int pos, int player);
void unmove(int pos, int player);
int isFull();
int hasWon(int player);
int winCheck(int player, unsigned long long mask, int shift);
unsigned long long potentialCheck(int player, unsigned long long mask, int shift);
unsigned long long getPotential(int player);
unsigned long long getHash();
__uint128_t getBoard();

#endif

