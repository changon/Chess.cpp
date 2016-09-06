/*ABBREVIATIONS*/
/*
	LSB - Least Significant Bit
*/
#include "BoardInit.h"

#include <stdio.h>//freopen, arrays, etc.
#include <stdint.h>//uint64_t
#include <vector>
#include <bitset>//converting bit integers to string format
#include <cstring>
#include <sstream>
#include <inttypes.h>//PRIx64
#include <string>
#include <iostream>//cerr, cout
#include <stdio.h>
#include <stdlib.h>

using namespace std;

/*
On the uint64_t board representation

from string conversion the LSB is shown to be the convertedString[63] = position A1. 

*/

///attack Arrays for non sliding pieces
uint64_t knightAttacks[64], kingAttacks[64], wpawnAttacks[64], bpawnAttacks[64], wpawnDAttacks[64], bpawnDAttacks[64];
	        
	        //vertical column files
	        const uint64_t afile = 0x0101010101010101;
	        const uint64_t bfile = 0x0202020202020202;
	        const uint64_t cfile = 0x0404040404040404;
	        const uint64_t dfile = 0x0808080808080808;
	        const uint64_t efile = 0x1010101010101010;
	        const uint64_t ffile = 0x2020202020202020;
	        const uint64_t gfile = 0x4040404040404040;
	        const uint64_t hfile = 0x8080808080808080;
	
	        //horizontal row ranks
	        const uint64_t rank1 = 0x00000000000000FF;
	        const uint64_t rank2 = 0x000000000000FF00;
	        const uint64_t rank3 = 0x0000000000FF0000;
	        const uint64_t rank4 = 0x00000000FF000000;
	        const uint64_t rank5 = 0x000000FF00000000;
	        const uint64_t rank6 = 0x0000FF0000000000;
	        const uint64_t rank7 = 0x00FF000000000000;
	        const uint64_t rank8 = 0xFF00000000000000;
		const uint64_t flags[64] = {
			0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400, 0x800, 0x1000, 0x2000, 0x4000, 0x8000, 0x10000, 0x20000, 0x40000, 0x80000, 0x100000, 0x200000, 0x400000, 0x800000, 0x1000000, 0x2000000, 0x4000000, 0x8000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000, 0x100000000, 0x200000000, 0x400000000, 0x800000000, 0x1000000000, 0x2000000000, 0x4000000000, 0x8000000000, 0x10000000000, 0x20000000000, 0x40000000000, 0x80000000000, 0x100000000000, 0x200000000000, 0x400000000000, 0x800000000000, 0x1000000000000, 0x2000000000000, 0x4000000000000, 0x8000000000000, 0x10000000000000, 0x20000000000000, 0x40000000000000, 0x80000000000000, 0x100000000000000, 0x200000000000000, 0x400000000000000, 0x800000000000000, 0x1000000000000000, 0x2000000000000000, 0x4000000000000000, 0x8000000000000000
		};		
		const uint64_t files[8] = {afile,bfile,cfile,dfile,efile,ffile,gfile,hfile};
		const uint64_t ranks[8] = {rank1,rank2,rank3,rank4,rank5,rank6,rank7,rank8};

//primary four arrays for magic and sliding move attacks	
uint64_t rookAttacks[64][4096];
//uint64_t rookMagics[64];
uint64_t bishopAttacks[64][512];
//extern uint64_t bishopMagics[64];
		uint64_t rookMagics[64]
		/* = {
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
		}*/;
		uint64_t bishopMagics[64] 
		/*= {
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
		}*/;//precomputed magics via generateMagics()
		
		const int OUT = 64;
				//rook borders; order of n e s w
		int rookBorders[64][4] = {
			{56,7,OUT,OUT},{57,7,OUT,0},{58,7,OUT,0},{59,7,OUT,0},{60,7,OUT,0},{61,7,OUT,0},{62,7,OUT,0},{63,OUT,OUT,0},
			{56,15,0,OUT},{},{},{},{},{},{},{63,OUT,7,8},
			{56,23,0,OUT},{},{},{},{},{},{},{63,OUT,7,16},
			{56,31,0,OUT},{},{},{},{},{},{},{63,OUT,7,24},
			{56,39,0,OUT},{},{},{},{},{},{},{63,OUT,7,32},
			{56,47,0,OUT},{},{},{},{},{},{},{63,OUT,7,40},
			{56,55,0,OUT},{},{},{},{},{},{},{63,OUT,7,48},
			{OUT,63,0,OUT},{OUT,63,1,56},{OUT,63,2,56},{OUT,63,3,56},{OUT,63,4,56},{OUT,63,5,56},{OUT,63,6,56},{OUT,OUT,7,56},
		};
		//bishop borders; order of noea,nowe, soea,sowe
		int bishopBorders[64][4] = {
			{63,OUT,OUT,OUT},{55,8,OUT,OUT},{47,16,OUT,OUT},{39,24,OUT,OUT},{31,32,OUT,OUT},{23,40,OUT,OUT},{15,48,OUT,OUT},{OUT,56,OUT,OUT},
			{62,OUT,1,OUT},{63,16,2,0},{55,24,3,1},{47,32,4,2},{39,40,5,3},{31,48,6,4},{23,56,7,5},{OUT,57,OUT,6},
			{61,OUT,2,OUT},{62,24,3,8},{63,32,4,0},{55,40,5,1},{47,48,6,2},{39,56,7,3},{31,57,15,13},{OUT,58,OUT,5},
			{60,OUT,3,OUT},{61,32,4,16},{62,40,5,8},{63,48,6,0},{55,56,7,1},{47,57,15,2},{39,58,23,3},{OUT,59,OUT,4},
			{59,OUT,4,OUT},{60,40,5,24},{61,48,6,16},{62,56,7,8},{63,57,15,0},{55,58,23,1},{47,59,31,2},{OUT,60,OUT,3},
			{58,OUT,5,OUT},{59,48,6,32},{60,56,7,24},{61,57,15,16},{62,58,23,8},{63,59,31,0},{55,60,39,1},{OUT,61,OUT,2},
			{57,OUT,6,OUT},{58,56,7,40},{59,57,15,32},{60,58,23,24},{61,59,31,16},{62,60,39,8},{63,61,47,0},{OUT,62,OUT,1},
			{OUT,OUT,7,OUT},{OUT,OUT,15,48},{OUT,OUT,23,40},{OUT,OUT,31,32},{OUT,OUT,39,24},{OUT,OUT,47,16},{OUT,OUT,55,8},{OUT,OUT,OUT,0},
		};
		
		//not used below
        	uint64_t bishopMask[64]={
        	        0x40201008040200, 0x402010080400, 0x4020100a00, 0x40221400, 0x2442800, 0x204085000, 0x20408102000, 0x2040810204000, 0x20100804020000,0x40201008040000, 0x4020100a0000, 0x4022140000, 0x244280000, 0x20408500000, 0x2040810200000, 0x4081020400000, 0x10080402000200, 0x20100804000400, 0x4020100a000a00, 0x402214001400, 0x24428002800, 0x2040850005000, 0x4081020002000, 0x8102040004000, 0x8040200020400, 0x10080400040800, 0x20100a000a1000, 0x40221400142200, 0x2442800284400,0x4085000500800, 0x8102000201000, 0x10204000402000, 0x4020002040800, 0x8040004081000, 0x100a000a102000, 0x22140014224000, 0x44280028440200, 0x8500050080400, 0x10200020100800, 0x20400040201000, 0x2000204081000, 0x4000408102000, 0xa000a10204000, 0x14001422400000, 0x28002844020000, 0x50005008040200, 0x20002010080400, 0x40004020100800, 0x20408102000, 0x40810204000, 0xa1020400000, 0x142240000000, 0x284402000000, 0x500804020000, 0x201008040200, 0x402010080400, 0x2040810204000, 0x4081020400000, 0xa102040000000, 0x14224000000000, 0x28440200000000, 0x50080402000000, 0x20100804020000, 0x40201008040200
        	};
        	uint64_t rookMask[64] ={
        	        0x101010101017e,
        	        0x202020202027c, 0x404040404047a, 0x8080808080876, 0x1010101010106e,
        	        0x2020202020205e, 0x4040404040403e, 0x8080808080807e, 0x1010101017e00,
        	        0x2020202027c00, 0x4040404047a00, 0x8080808087600, 0x10101010106e00,
        	        0x20202020205e00, 0x40404040403e00, 0x80808080807e00, 0x10101017e0100,
        	        0x20202027c0200, 0x40404047a0400, 0x8080808760800, 0x101010106e1000,
        	        0x202020205e2000, 0x404040403e4000, 0x808080807e8000, 0x101017e010100,
        	        0x202027c020200, 0x404047a040400, 0x8080876080800, 0x1010106e101000,
        	        0x2020205e202000, 0x4040403e404000, 0x8080807e808000, 0x1017e01010100,
        	        0x2027c02020200, 0x4047a04040400, 0x8087608080800, 0x10106e10101000,
        	        0x20205e20202000, 0x40403e40404000, 0x80807e80808000, 0x17e0101010100,
        	        0x27c0202020200, 0x47a0404040400, 0x8760808080800, 0x106e1010101000,
        	        0x205e2020202000, 0x403e4040404000, 0x807e8080808000, 0x7e010101010100,
        	        0x7c020202020200, 0x7a040404040400, 0x76080808080800, 0x6e101010101000,
        	        0x5e202020202000, 0x3e404040404000, 0x7e808080808000, 0x7e01010101010100,
        	        0x7c02020202020200, 0x7a04040404040400, 0x7608080808080800, 0x6e10101010101000,
        	        0x5e20202020202000, 0x3e40404040404000, 0x7e80808080808000
        	};
		
/*BoardInit()
{
	setup();
}

~BoardInit()
{
	
}*/
/*
//practice with pointers
uint64_t** transferMagics()
{
	uint64_t** ret = new uint64_t*[2];
	for (int i =0; i <2;i++)
	{
		ret[i] = new uint64_t[64];
	}
	ret[0]=rookMagics;
	ret[1]=bishopMagics;
	return ret;
}

uint64_t*** transferAttacks()
{
	uint64_t*** ret = new uint64_t**[2];
	for (int i =0; i< 2; i++)
	{
		ret[i]= new uint64_t*[64];
		for (int j =0; j < 64; j++)
		{
			ret[i][j] = new uint64_t[4096];
		}
	}
	for (int i =0; i< 64; i++)
	{
		ret[0][i]= rookAttacks[i];
		ret[1][i]= bishopAttacks[i];
	}
	return ret;
}*/

void printString(uint64_t board){
        bitset<64>b (board);
        string tmp = b.to_string();
	int markers [8] = {63, 55, 47, 39, 31, 23, 15, 7}; 
	for (int i = 7; i > -1; i--)
	{
		int k = markers[i];
		for (int j =0; j < 8; j ++){
			cout << tmp[k]<<" ";
			k--;
		}
		cout << endl;
	}
        //printf("String is \n%s\n String done printing.", tmp.c_str());
	cout<<endl;
}

/**
	@brief Tom Romstad's method for generating random 64 bit int
*/
uint64_t get_rand_64bit()
{	
	uint64_t u1 = (uint64_t)(random()) & 0xFFFF; 
	uint64_t u2 = (uint64_t)(random()) & 0xFFFF;
	uint64_t u3 = (uint64_t)(random()) & 0xFFFF; 
	uint64_t u4 = (uint64_t)(random()) & 0xFFFF;
	return u1 | (u2 << 16) | (u3 << 32) | (u4 << 48);
}

int magicMapping(uint64_t magic, uint64_t blocker, int shift)
{
	return (int)((magic*blocker)>>(64-shift));
}

uint64_t getMagic(int square, int shift, bool isRook, uint64_t mask)//shift is the number of bits
{
	int used[4096]; //keep track of which indices are used
	uint64_t attackSet[4096], blockers[4096], blockerAttack[4096];		
	
	//set the blockers array and the blockerAttack array
	for (int i=0;i<(1ULL<<shift);i++)
	{			
		blockers[i] = generateCombo(i, shift, mask);
		blockerAttack[i] = computeSliderAttack(blockers[i], isRook, square);
	}
	
	for (int i =0;i<100000000;i++)//try 100 million 64bit integers for magic
	{
		for (int j=0;j<4096; j++)//reset used mappings so that nothing is 'used', same with the attackSet
		{
			used[j]=0;
			attackSet[j] = 0;
		}
		uint64_t candidateMagic= (get_rand_64bit() & get_rand_64bit() & get_rand_64bit());//get candidate magic
		bool failed = false;
		for (int j =0;( (j<(1ULL<<shift)) && (!failed) );j++)//go through each combination of blockers for the given square
		{
			uint64_t blocker = blockers[j];
			int index = magicMapping(candidateMagic, blocker,shift);//get magic mapping onto the 0-2^n possible indices
			uint64_t attack = blockerAttack[j]; 
			if (isRook)
			{
				rookAttacks[square][index]=attack;//set in the actual attack table
			}
			else if (!isRook) 
			{
				bishopAttacks[square][index]=attack;
			}
			if (used[index] !=0)//if index here already used check if it holds the same attack table
			{
				//check if attack set here is the same as the required mapping
				if (!(attackSet[index] == attack))//if they are not the same, failed magic
					failed = true;//here we have a collision and perfect hashing fails
			}
			else if (used[index] == 0)//if this index has not been mapped to yet
			{
				attackSet[index] = attack;//store the relevant attack associated with the blocker j and magicMapped index
				used[index] = 1;//mark this index as used
			}
		}
		if (!failed)
		{	
			//set the magic for the given square, setting the attack table done in getMagic if magic found
			if(isRook)
				rookMagics[square] = candidateMagic;
			else if(!isRook)
				bishopMagics[square]=candidateMagic;
		/*	
		uint64_t magic, block, at;
		int combo = 15;
		if (isRook)
		{
			block = blockers[combo];
		}
		else if(!isRook){
			block = blockers[combo];
		}

		cout<<endl <<"Test square "<< square<<endl;
		int index = magicMapping(candidateMagic, block, shift);

		if(isRook)
			at=rookAttacks[square][index];
		else if(!isRook)
			at=bishopAttacks[square][index];
		cout<<"Blocker"<<endl;
		printString(block);
		cout<<"Attack set"<<endl;
		printString(at);
		cout<<"index is "<<index<<endl;
		*/
			return candidateMagic;//Magic is found!
		}
	}
	return (0x1111111111111111);//this should not happen, if it does, magic is not found
}

void generateMagics()
{
	srand(time(0));//seed for random 64 bit integer generation
	cout<<endl<<"Rooks."<<endl;
	//rooks first
	for (int i =0; i < 64; i ++)//square
	{
		int j =0;
		uint64_t magic = getMagic(i, RBits[i], true, rookMask[i]);
		if (magic == 0x1111111111111111)
		{
			cerr<<"Magic not found for square "<<i<<endl;
		}
		else {
			//cout <<"Rook Square "<<i<<": "<<magic<<endl;
			/*printf("0x%" PRIx64", ", magic);
			if (i%4==0 && i != 0)
				printf("\n");
			*/
		}
	}
	cout<<endl<<"Bishops."<<endl;

	for (int i =0; i < 64; i ++)//square
	{
		int j =0;
		uint64_t magic = getMagic(i, BBits[i], false, bishopMask[i]);
		if (magic == 0x1111111111111111)
		{
			cerr<<"Magic not found for square "<<i<<endl;
		}
		else {
			//cout <<"Bishop Square "<<i<<": "<<magic<<endl;
			/*printf("0x%" PRIx64", ", magic);
			if (i%4==0 && i != 0)
				printf("\n");
			*/
		}
	}
}

/**
	@brief Get the attack table for a rook or bishop
*/
uint64_t computeSliderAttack(uint64_t blocker, bool isRook, int square)
{
	//follow flood fill approach
	if (isRook)
	{
		uint64_t north = floodFillPlus(blocker, 8, square, rookBorders[square][0]);
		uint64_t east = floodFillPlus(blocker, 1, square, rookBorders[square][1]);
		uint64_t south = floodFillMinus(blocker, 8, square, rookBorders[square][2]);
		uint64_t west = floodFillMinus(blocker, 1, square, rookBorders[square][3]);
		uint64_t attack = (north | east | south | west);
		return attack;
	}
	else//if a bishop 
	{
                uint64_t noea = floodFillPlus(blocker, 9, square, bishopBorders[square][0]);//north east
	        uint64_t nowe = floodFillPlus(blocker, 7, square, bishopBorders[square][1]);//north west
                uint64_t soea = floodFillMinus(blocker, 7, square, bishopBorders[square][2]);//south eas
                uint64_t sowe = floodFillMinus(blocker, 9, square, bishopBorders[square][3]);//south west
                uint64_t attack = (nowe | noea | sowe | soea);
                return attack;	
	}
}

/**
	@brief Essentially left shift check
*/
uint64_t floodFillPlus(uint64_t blocker, int directionalShift, int square, int border)
{
        //set flood
        uint64_t flood = 0;
        if(border==OUT)//if there is no border in this direction, return
		return flood;

        uint64_t slider = (1ULL<<square);
	uint64_t empty = ~blocker;
	int position = square;
	bool atEdge=false;
	int i =0; 
        while (i < 8)
        {        
		slider <<= directionalShift;   
                position+=directionalShift;
		if (atEdge)//if border for square reached, break out of loop and return progress
			return flood;
		if (border==position)
			atEdge=true;
		flood |= (slider & empty);
		if ((slider & blocker) > 0)
			return flood | (slider & blocker);
		i++;
	}
	return flood;
}

/**
	@brief Essentially right shift check
*/
uint64_t floodFillMinus(uint64_t blocker, int directionalShift, int square, int border)
{
        uint64_t slider = (1ULL<<square);
        //set flood
        uint64_t flood = 0;
        if(border==OUT)//if there is no border in this direction, return
		return flood;

	uint64_t empty = ~blocker;
	int position = square;
	bool atEdge=false;
	int i =0; 
        while (i < 8)
        {        
		slider >>= directionalShift;//shift to next sliding position
                position-=directionalShift;//update integer position of board as followed by LERF
		if (atEdge)//if border for square reached, break out of loop and return progress
			return flood;
		if(border==position)//if at edge, mark boolean to break out next go around
			atEdge=true;
		flood |= (slider & empty);//flood the flood
		if ((slider & blocker) > 0)//if the slider and blocker mask share a position, return the position with it to get edges
			return (flood | (slider & blocker));
		i++;
        }
	return flood;	
}

/**
	@brief Generates all combinations for rook and bishop masks, along with their respective attack tables. Stored into state arrays. In addition, the borders for rook inner squares are computed as determined by LERF square mapping for chessboards.
*/
void setBorders()
{
	int square =0;
	for (int i =0;i<8;i++)
	{
		for (int j =0;j<8;j++)
		{
			//set the rook borders for each square
			if (i>0 && j>0 && i<7 && j<7)//not corner/edge board space; Corner/edge board space precomputed limits
			{
				rookBorders[square][0] = square + (8*(7-i));//north border
				rookBorders[square][1] = square + (1*(7-j));//east
				rookBorders[square][2] = square - (8*i);//south
				rookBorders[square][3] = square - (1*j);//west
			}
			square ++;
		}
	}
}

//Tom Romstad's method for generating combinations of occupancy masks
uint64_t generateCombo(int combo, int nbits, uint64_t mask)
{
	int i,j;
	uint64_t result = 0;
	for (i=0;i<nbits;i++)
	{
		j=pop_1st_bit(&mask);
		if(combo & (1<<i))
			result |= (1ULL<<j);
	}
	return result;
}

int pop_1st_bit(uint64_t *board)
{
	uint64_t b = *board^(*board-1);
	unsigned int fold = (unsigned) ((b & 0xffffffff)^(b>>32));
	*board &= (*board-1);
	return BitTable[(fold*0x783a9b23)>>26];
}

///check if a given rank or file is in range to be computed
bool inRange(int n){ return (!(n<0 || n>7)); }

///Arrays automatically pass by reference
///Used to precompute attack tables for nonsliding pieces; a perfect hash may be given by ffs() on a bitstring with given piece position
void setAttack(int rankoffset[], int fileoffset[], uint64_t destination[], int size){
	int attackIndex =0;
	for (int i =0; i<8; i++)
	{
		for (int j =0;j< 8; j++)
		{
			//get files one and two to left/right
			//get ranks one and two to left/right
			uint64_t attacks[8];
			int index=0;
			//go through the max eight positions
			for (int k=0;k<size;k++)
			{
				int rank = i+rankoffset[k];
				int file = j+fileoffset[k];		
				if (inRange(rank) && inRange(file)){
					attacks[index] = (files[file] & ranks[rank]);
					index++;
				}
			}
			uint64_t allAttack;
			
			//XOR all the attack positions together for full attack table of a given position
			if(index > 0)//if valid moves produced
			{
				allAttack = attacks[0];
				for (int k = 1; k<index; k++)
					allAttack ^= attacks[k];
			}
			//attack table for knight has been computed. now add this to an array to be hashed from for position
			destination[attackIndex] = allAttack;
			//printString(allAttack);
			attackIndex++;//go 0-63		
		}
	}
}

void setKnightAttack(){
	//get files one and two to left/right
	//get ranks one and two to left/right
	int tmp1[8]={1, 1, -1, -1, 2, 2, -2, -2};
	int tmp2[8]={2, -2, 2, -2, 1, -1, 1, -1};
	setAttack(tmp1, tmp2, knightAttacks, 8);
}

///Similar to the KnightAttack, but it is importatn to note the first rank and eighth rank is not occupiable by white and black respectively
void setPawnAttack(){
	//Set White
	int fileForward[1] = {0};
	int wrankForward[1] = {1};
	int brankForward[1] = {-1};
	
	int fileDiagonal[2] = {1, -1};
	int wrankDiagonal[2] = {1, 1};
	int brankDiagonal[2] = {-1,-1};
	
	setAttack(wrankDiagonal, fileDiagonal, wpawnDAttacks, 2);//call wpawnDiagonal and & it with enemy pieces
	setAttack(brankDiagonal, fileDiagonal, bpawnDAttacks, 2);
	setAttack(wrankForward, fileForward, wpawnAttacks, 1);
	setAttack(brankForward, fileForward, bpawnAttacks, 1);

	//call wpawnDiagonal and & it with enemy pieces
	//forward attack, add in the double leap forward on first move
	for (int i =0; i< 64;i++)
	{
		//get current attack set and <</>> depending on black/white to get the next position forward
		//bpawnAttacks
		if(i>=48 && i<=55)
			bpawnAttacks[i]^= (bpawnAttacks[i]>>8);
		//wpawnAttacks
		if(i>=8 && i<=15)
			wpawnAttacks[i]^=(wpawnAttacks[i]<<8);
	}
}

void setKingAttack(){
	int rankoffset[8] = {0,0, 1,1,1,-1,-1,-1};
	int filesoffset[8]= {1,-1,0,1,-1,0,1,-1};
	setAttack(rankoffset, filesoffset, kingAttacks, 8);
}

void aborting()
{
	cout << "Aborting..."<<endl;
	abort();
}

void setup()
{
	//Compute Masks for bishop, rook, queen, king, knight
	setBorders();
	//generateMagics();
	setKingAttack();
	setPawnAttack();
	setKnightAttack();
}
