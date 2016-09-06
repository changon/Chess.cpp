#ifndef BOARD_H
#define BOARD_H

#include "Move.h"
#include "Info.h"
#include <vector>
#include <string>
#include <inttypes.h>

class Board{
	public:
		/*Constructor*/
		Board();
	
		/*Public Methods*/
		void printString(uint64_t board);
		void printBoard(uint64_t board);
		void display();
		std::vector <Move*> getMoves(std::string color);
		bool checkExists(std::string color);
		void humanMove();
		void play(int depth);
		void play();		

	private: 
		/*Private State Variables*/
		uint64_t wpawn, wknight, wbishop, wrook, wqueen, wking;
		uint64_t bpawn, bknight, bbishop, brook, bqueen, bking;
		uint64_t woccupation, boccupation;

		uint64_t wpawnForwardAtt[64];
		uint64_t wpawnDiagonalAtt[64];
		uint64_t bpawnForwardAtt[64];
		uint64_t bpawnDiagonalAtt[64];

		uint64_t wpawnAtt[64];
		uint64_t bpawnAtt[64];
		uint64_t knightAtt[64];
		uint64_t bishopAtt[64];
		uint64_t kingAtt[64];
		uint64_t rookAtt[64];
		uint64_t queenAtt[64];
		
		uint64_t rookOccMask[64];
		uint64_t bishopOccMask[64];
		uint64_t queenOccMask[64];//alternatively XOR this as you go, but storing it seperately is fine

		std::vector <uint64_t> boards;
		/*
			wpawn, wknight, wbishop, wrook, wqueen, wking
			bpawn, bknight, bbishop, brook, bqueen, bking
		*/
		uint64_t ranks [8];
		uint64_t files [8];
		uint64_t afile, bfile, cfile, dfile, efile, ffile, gfile, hfile;//for masking during attack table generation
		uint64_t rank1, rank2, rank3, rank4, rank5, rank6, rank7, rank8;//for masking during attack table gen
		uint64_t flags[64];//for 1,2,4,8 and so on to represent our board
		
		/*Private Methods*/

		///Bitwise
		uint64_t reverse(uint64_t reversee);
		uint64_t floodRight(uint64_t slider, uint64_t blocker, int shift);
		uint64_t floodLeft(uint64_t slider, uint64_t blocker, int shift);
		uint64_t getRookAttack(int sq, uint64_t blocker);
		uint64_t getBishopAttack(int sq, uint64_t blocker);

		///et cetera.
		void breakPoint();
		void setup();
		void setAttack(int rankoffset[], int fileoffset[], uint64_t destination[], int size);
		void setKnightAttack();
		void setPawnAttack();
		void setKingAttack();
		void setBishopAttack();
		void setRookAttack();
		void setQueenAttack();
		bool inRange(int n);
		bool inRangeMagic(int n);
		int max(int i, int j);
		double alphaBeta(Info info, std::string color, int depth, double alpha, double beta);		
		void makeMove(Move* mv);
		void undoMove(Move* mv);
};

#endif
