#include "Board.h"

#include <stdint.h>//uint64_t
#include <bitset>
#include <string>

using namespace std;

/*Rook/bishop*/
extern uint64_t rookAttacks[64][4096];
extern uint64_t bishopAttacks[64][512];
extern uint64_t rookMagics[64], bishopMagics[64], knightAttacks[64], kingAttacks[64], wpawnAttacks[64], bpawnAttacks[64], wpawnDAttacks[64], bpawnDAttacks[64];

Board::Board(/*uint64_t** magics, uint64_t*** attacks*/)
{
/*	for (int i =0; i<64;i++)
	{
		rookMagics[i] = magics[0][i];
		bishopMagics[i]=magics[1][i];
		for (int j =0; j< (1ULL<<RBits[j]);j++)
			rookAttacks[i][j]= attacks[0][i][j];
		for (int j =0;j<(1ULL<<BBits[j]);j++)
			bishopAttacks[i][j]=attacks[1][i][j];
	}*/
        int square = 5;
        uint64_t bishop = 0x201000000000;
        uint64_t rook = 0x30;
        printString(bishop);
        printString(bishopAttacks[square][magicMapping(bishopMagics[square],bishop, BBits[square])]);
        printString(rook);
        printString(rookAttacks[square][magicMapping(rookMagics[square], rook, RBits[square])]);
		printf("done testing\n");
		printf("non sliders test\n");
		printString(wpawnAttacks[square]);
		printString(bpawnAttacks[square]);//something wrong with black pawn attacks
		printString(wpawnDAttacks[square]);
		printString(bpawnDAttacks[square]);
		printString(knightAttacks[square]);
		printString(kingAttacks[square]);

}

int Board::magicMapping(uint64_t magic, uint64_t blocker, int shift)
{
	return (int)(magic*blocker>>(64-shift));
}

//print board with UNICODE pieces
void Board::display()
{
        int count = 63;
        for (int i =0; i < 8; i ++)
        {
                printf("\n");
                for (int j=0;j <8;j++)
                {
                        int bnum = -1;//for empty board space
                        uint64_t position = (1ULL<<count);
                        for (int i=0; i < 12; i++)
                                if((boards[i] & position) > 0)
                                {
                                        bnum =i;
                                        break;
                                }
                        switch (bnum){
                                case 0: { printf("\u2659 "); break; }
                                case 1: { printf("\u2658 "); break; }
                                case 2: { printf("\u2657 "); break; }
                                case 3: { printf("\u2656 "); break; }
                                case 4: { printf("\u2655 "); break; }
                                case 5: { printf("\u2654 "); break; }
                                case 6: { printf("\u265F "); break; }
                                case 7: { printf("\u265E "); break; }
                                case 8: { printf("\u265D "); break; }
                                case 9: { printf("\u265C "); break; }
                                case 10:{ printf("\u265B "); break; }
                                case 11:{ printf("\u265A "); break; }
                                default: printf(" ");
                        }
                        count--;
                }
        }
        printf("\n");
}

void Board::printString(uint64_t board){
        bitset<64>b (board);
        string tmp = b.to_string();
        int markers [8] = {63, 55, 47, 39, 31, 23, 15, 7};
        for (int i = 7; i > -1; i--)
        {
                int k = markers[i];
                for (int j =0; j < 8; j ++){
			printf("%c ", tmp[k]);
                        k--;
                }
                printf("\n");
        }
        printf("\n");
}
