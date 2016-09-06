#ifndef BOARDINIT_H
#define BOARDINIT_H

#include <vector>
#include <stdint.h>

using namespace std;

//class BoardInit{
	//public:
		//BoardInit();
		//~BoardInit();//destructor
		void printString(uint64_t board);
//		uint64_t** transferMagics();//boards for rookMagics[],bishopMagics[]
//		uint64_t*** transferAttacks();//boards for rookAttacks[][],bishopAttacks[][]
		
	//private:
		/*"Private" State Variables*/
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

//                uint64_t rookFullMask[64];//full masks without shaved off edges; current position need not be subtracted out as it is only & with flood fill
//		uint64_t bishopFullMask[64];

                /*
                        wpawn, wknight, wbishop, wrook, wqueen, wking
                        bpawn, bknight, bbishop, brook, bqueen, bking
                */
/*                uint64_t ranks [8];
                uint64_t files [8];
                uint64_t afile, bfile, cfile, dfile, efile, ffile, gfile, hfile;//for masking during attack table generation
                uint64_t rank1, rank2, rank3, rank4, rank5, rank6, rank7, rank8;//For masking during attack table gen
                uint64_t flags[64];//for 1,2,4,8 and so on to represent our board
*/
		const int BitTable[64] = { 
		  63, 30, 3, 32, 25, 41, 22, 33, 15, 50, 42, 13, 11, 53, 19, 34, 61, 29, 2,
		  51, 21, 43, 45, 10, 18, 47, 1, 54, 9, 57, 0, 35, 62, 31, 40, 4, 49, 5, 52, 
		  26, 60, 6, 23, 44, 46, 27, 56, 16, 7, 39, 48, 24, 59, 14, 12, 55, 38, 28, 
		  58, 20, 37, 17, 36, 8
		};

		//*Private State methods*/
		///maintenance methods
		void setBorders();

		/*Magic BB Methods*/
		uint64_t generateCombo(int combo, int nbits, uint64_t mask);//followed as index_to_uint64() by Tom Romstad
		int pop_1st_bit(uint64_t *board);
		uint64_t computeSliderAttack(uint64_t blocker, bool isRook, int square);
		uint64_t floodFillPlus(uint64_t blocker, int directionalShift, int square, int border);//left directional shifts
		uint64_t floodFillMinus(uint64_t blocker, int directionalShift, int square, int border);//right directional shifts
		void generateMagics();
		uint64_t getMagic(int square, int shift, bool isRook, uint64_t mask);
		int magicMapping(uint64_t magic, uint64_t blocker, int shift);//get a mapping to an index from a magic number	
		uint64_t get_rand_64bit();

        	/*uint64_t rookMagics[64] = {
		        0xa8002c000108020, 0x4440200140003000, 0x8080200010011880, 0x380180080141000, 0x1a00060008211044, 
			0x410001000a0c0008, 0x9500060004008100, 0x100024284a20700, 0x802140008000, 
			0x80c01002a00840, 0x402004282011020, 0x9862000820420050, 0x1001448011100, 
			0x6432800200800400, 0x40100010002000c, 0x2800d0010c080, 0x90c0008000803042, 
			0x4010004000200041, 0x3010010200040, 0xa40828028001000, 0x123010008000430, 
			0x24008004020080, 0x60040001104802, 0x582200028400d1, 0x4000802080044000, 
			0x408208200420308, 0x610038080102000, 0x3601000900100020, 0x80080040180, 
			0xc2020080040080, 0x80084400100102, 0x4022408200014401, 0x40052040800082, 
			0xb08200280804000, 0x8a80a008801000, 0x4000480080801000, 0x911808800801401, 
			0x822a003002001894, 0x401068091400108a, 0x4a10a00004c, 0x2000800640008024, 
			0x1486408102020020, 0x100a000d50041, 0x810050020b0020, 0x204000800808004, 
			0x20048100a000c, 0x112000831020004, 0x9000040810002, 0x440490200208200, 
			0x8910401000200040, 0x6404200050008480, 0x4b824a2010010100, 0x4080801810c0080, 
			0x400802a0080, 0x8224080110026400, 0x40002c4104088200, 0x1002100104a0282, 
			0x1208400811048021, 0x3201014a40d02001, 0x5100019200501, 0x101000208001005, 
			0x2008450080702, 0x1002080301d00c, 0x410201ce5c030092	
		};
		uint64_t bishopMagics[64] = {
			0x40210414004040, 0x2290100115012200, 0xa240400a6004201, 0x80a0420800480, 0x4022021000000061, 
			0x31012010200000, 0x4404421051080068, 0x1040882015000, 0x8048c01206021210, 
			0x222091024088820, 0x4328110102020200, 0x901cc41052000d0, 0xa828c20210000200, 
			0x308419004a004e0, 0x4000840404860881, 0x800008424020680, 0x28100040100204a1, 
			0x82001002080510, 0x9008103000204010, 0x141820040c00b000, 0x81010090402022, 
			0x14400480602000, 0x80000104e0a1840, 0x8a008048443c00, 0x400820200820c140, 
			0x5141042202600a2, 0x90021a040113, 0x6100400104010a0, 0x20840000802008, 
			0x40050a010900a080, 0x818404001041602, 0x8040604006010400, 0x1028044001041800, 
			0x80b00828108200, 0xc000280c04080220, 0x3010020080880081, 0x10004c0400004100, 
			0x3010020200002080, 0x202304019004020a, 0x4208a0000e110, 0x108018410006000, 
			0x202210120440800, 0x100850c828001000, 0x1401024204800800, 0x41028800402, 
			0x20642300480600, 0x20410200800202, 0xca02480845000080, 0x140c404a0080410, 
			0x2180a40108884441, 0x4410420104980302, 0x1108040046080000, 0x8141029012020008, 
			0x894081818082800, 0x40020404628000, 0x804100c010c2122, 0x8168210510101200, 
			0x1088148121080, 0x204010100c11010, 0x1814102013841400, 0xc00010020602, 
			0x1045220c040820, 0x12400808070840, 0x2004012a040132	
		};//precomputed magics via generateMagics()*/

		/*NonSlidingPiece Attack Generation*/
		void setAttack(int rankoffset[], int fileoffset[], uint64_t destination[], int size);
		void setKnightAttack();
		void setPawnAttack();
		void setKingAttack();
		bool inRange(int n);

		/*Etc.*/
		void setup();
		void aborting();

//};

#endif
