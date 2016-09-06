#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>

class Board{
	public: 
		Board();
		void display();
		void printString(uint64_t board);
	private:
		///Private State Variables
		/*Board State*/
		
	        //set initial pieces, update later
	        uint64_t woccupation = 0x000000000000FFFF;
	        uint64_t boccupation = 0xFFFF000000000000;
	
	        uint64_t wpawn = 0x000000000000FF00;
	        uint64_t wknight=0x0000000000000042;
	        uint64_t wbishop=0x0000000000000024;
	        uint64_t wrook = 0x0000000000000081;
	        uint64_t wqueen =0x0000000000000008;
	        uint64_t wking = 0x0000000000000010;

	        uint64_t bpawn = 0x00FF000000000000;
	        uint64_t bknight=0x4200000000000000;
	        uint64_t bbishop=0x2400000000000000;
	        uint64_t brook = 0x8100000000000000;
	        uint64_t bqueen =0x0800000000000000;
	        uint64_t bking = 0x1000000000000000;

		//post maintenance, store into an array with all boards to update mv by mv
		uint64_t boards[14] = {
			wpawn, wknight, wbishop, wrook, wqueen, wking,
			bpawn, bknight, bbishop, brook, bqueen, bking,
			woccupation, boccupation
		};

        	const int RBits[64] = {
                  12, 11, 11, 11, 11, 11, 11, 12,
                  11, 10, 10, 10, 10, 10, 10, 11,
                  11, 10, 10, 10, 10, 10, 10, 11,
                  11, 10, 10, 10, 10, 10, 10, 11,
                  11, 10, 10, 10, 10, 10, 10, 11,
                  11, 10, 10, 10, 10, 10, 10, 11,
                  11, 10, 10, 10, 10, 10, 10, 11,
                  12, 11, 11, 11, 11, 11, 11, 12
                };
                const int BBits[64] = {
                  6, 5, 5, 5, 5, 5, 5, 6,
                  5, 5, 5, 5, 5, 5, 5, 5,
                  5, 5, 7, 7, 7, 7, 5, 5,
                  5, 5, 7, 9, 9, 7, 5, 5,
                  5, 5, 7, 9, 9, 7, 5, 5,
                  5, 5, 7, 7, 7, 7, 5, 5,
                  5, 5, 5, 5, 5, 5, 5, 5,
                  6, 5, 5, 5, 5, 5, 5, 6
                };

		///Private State Methods
		int magicMapping(uint64_t magic, uint64_t blocker, int shift);
};

#endif
