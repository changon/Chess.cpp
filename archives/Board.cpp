#include "Board.h"
#include "Move.h"
#include "Play.h"
#include "Info.h"
#include <bitset>
#include <stdlib.h> //abort(), strtoull()
#include <vector>
#include <cmath>
#include <string>//to_string(), strcpy()
#include <cstring>
#include <sstream>
#include <cstdint>//reinterpret_cast()
#include <inttypes.h> //printf("%" PRIu8 "\n", bit8int);

using namespace std;

int bound = 7;//for how far to go into getMoves getting the movelist going across each piece

/*
Extract certain bits. Creds to:
http://stackoverflow.com/questions/10090326/how-to-extract-specific-bits-from-a-number-in-c
unsigned short extract(unsigned short value, int begin, int end)
{
    unsigned short mask = (1 << (end - begin)) - 1;
    return (value >> begin) & mask;
}
*/

/*
	TODO
		search tree
		pawnAttack to diagonal attack
		Mv make/unmake
		getMoves()
			Magic bitboard mv generation for sliding pieces
			Sliding blocker masks, get rid of edge bit toggles

		Zobrist hashing for transposition tables

		checkExists()
		Info structure for mv by mv

	NOTE:
		Board is like this with respect to the bitstring of a unsigned 64 bit

		63 62 61 60 59 58 57 56
		55 54 53 52 51 50 49 48
		47 46 45 44 43 42 41 40
		39 38 37 36 35 34 33 32
		31 30 29 28 27 26 25 24
		23 22 21 20 19 18 17 16
		15 14 13 12 11 10 9  8 
		7  6  5  4  3  2  1  0 
			
		A8 B8................H8
		.		      .
		.		      .
		.		      .
		. 		      .
		.		      .
		.		      .
		A1 B1................H1	
*/

Board::Board()
{
	setup();
	
	///set up search tree parameters	
	int depth = 9;
	int test = 1;
	//play(test);
}

void Board::play(int depth){
	bool alive = true;
	int counter = 0;
	Info info;
	
	while (alive && counter ==0)
	{
		display();
		if (counter %2 == 1)//black to go
			double value = alphaBeta(info, "BLACK", depth, (double)-9999999, (double)9999999);//initial  "infinity" and ninfinity arbitrary
		else if (counter %2 ==0)
			double value = alphaBeta(info, "WHITE", depth, (double)-9999999, (double)9999999);//initial  "infinity" and ninfinity arbitrary
		
		// humanMove();
		
		counter++;
	}
}

void Board::play(){
	play(MAX_DEPTH);
}

void Board::humanMove(){
	//take in terminal input via makeMove() or UCI communication
	printf("human turn\n");
}

///must implement
bool Board::checkExists(string Color){
	return false;
}

double Board::alphaBeta(Info info, std::string color, int depth, double alpha, double beta){
	///info here will hold 'etc' information, passed just for structure and integrity
	bool terminal = false;

	///instance of Evaluation class should be called

	if (depth == 0) 
		return (double)10;//here we would want to perform quiescence search to avoid the horizontal effect using a non-trained evaluation fx
	//if TERMINAL NODE, return large + value for winning (or - Losing)
	if (terminal) 
		return (double) 10000;// negative reward depending on loss or win, this is variable also, not just the arbitrary 10000

	string nextLoop;
	if (color == "WHITE") 
		nextLoop = "BLACK";
	else if (color == "BLACK") 
		nextLoop = "WHITE";

	//order moves: in a non-trained evaluation fx, follow killer heuristics, etc.

	vector <Move*> mvlist = getMoves(color);
	for (int i = 0; i < mvlist.size(); i++)
	{		
		//makemove
		//printf("make mv\n");
		makeMove(mvlist[i]);

		int value = -alphaBeta(info, nextLoop, depth-1, -beta, -alpha);
		if (value > alpha)
			alpha = value;
		if (value >= beta)
			return beta;//beta cutoff here

		//undomove
		undoMove(mvlist[i]);
	}
	return alpha;
}

void Board::makeMove(Move* move){
	uint64_t src = move->getSource();
	uint64_t des = move->getDestination();
	string colore = move->getColor();
	int startType = move->getType();

	uint64_t frand, enemy;

	int enemyStart, enemyEnd;
	
	//frand and enemy should be by reference & to modify address as java does
	if (colore== "WHITE")
	{
		frand = boards[12];
		enemy = boards[13];
		enemyStart = 6; enemyEnd =12;
	}
	else if (colore == "BLACK")
	{
		frand = boards[13];
		enemy = boards[12];
		enemyStart =0; enemyEnd = 6;
	}

	///take out starting position, add in ending
	frand -= src;	
	frand ^= des;

	///check position at destination
	int endingType = -1;
	
	for (int i =enemyStart; i < enemyEnd; i ++)
	{
		if (des & boards[i])
		{
			endingType = i;
			break;
		}
	}

	//simply add destination there to update start type
	//boards[startType] += des;
	boards[startType] ^= des;

	move->setOldType(endingType);

	///however, if there is a capture, rm from that position
	if (endingType != -1)
	{
		//rm from endingType destination
		boards[endingType] -= des;
		//take out enemy destination position here (if it exists)
		enemy -= des;
	}
	/*
	Summary of move make and unmake
	    make:
		take occupancy
		substract from boards[index]
		add destination to boards[index]
		loop 1-12 & destination to see if another piece exists at destination
			save where the piece into 'oldtype'
		
	    unmake:
		subtract destination from boards[index]
		add src to boards[index[
		add destination to boards[oldtype]		

		keep track where ending is through global var since alpha/beta returns the value naturally
	*/
}

void Board::undoMove(Move* move){
	int startType = move->getType();
	int oldType = move->getOldType();
	uint64_t src = move->getSource();
	uint64_t des = move->getDestination();
	string colore = move->getColor();
	uint64_t frand, enemy;

        if (colore== "WHITE")
        {
                frand = boards[12];
                enemy = boards[13];
        }
        else if (colore == "BLACK")
        {
                frand = boards[13];
                enemy = boards[12];
        }


	///rm ending position
	boards[startType] -= des;
	///reinstate starting position
	boards[startType] ^= src;

	///if old piece existed at destination, reinstate
	if (oldType != -1)
	{
		boards[oldType] ^=des;
		enemy ^= des;// add des if it existed
	}
	frand ^=src;//add in frand src
	frand -= des;// take out des
}

vector <Move*> Board::getMoves(string Color){
	vector <Move*> mvlist;
	//char** hello = getBishopSlideR(arg);
	//delete [] hello	
	///here check for checking forced moves, while multiple moves may be available for segmentation of program and organization, i include this as an initial seperate component
	//if (!checkExists(Color))	return;//if we are in check, no moves to make but few
	
	uint64_t friendly = ((Color == "WHITE") ? boards[12] : boards[13]);
	uint64_t enemy = ((Color == "BLACK") ? boards[12] : boards[13]);

	int exists = 1000;//updated via ffs

	int COUNT = 0;		
	///here we assume that one mv can always be made for a given side during their start by setting exists to 1000 nearly doing a do-while loop
	while (exists != 0){
		//here it is named reference because the friendly updated as moves are iterated through are inaccurate
		uint64_t refFriendly = ((Color == "WHITE") ? boards[12] : boards[13]);
		uint64_t refEnemy = ((Color == "BLACK") ? boards[12] : boards[13]);
		printf("\n\nNEW ITERATION\n");
		printBoard(friendly);
		printf("\nfriendly above\n");
		int shiftAndPosition = ffs(friendly); //least significant bit representing how to shift to and with the boards[] and to index the precomputed attack masks
		uint64_t occupancy = 0x0000000000000000;
		
		///set the occupation bit alone to use as a marker

		occupancy |= 1<<(shiftAndPosition-1);
		
		///now loop through the occupation to get what type of piece it is, and get specific attack board based on this
		int type = 0;
		for (int i =0; i< 12; i++)
			if ((boards[i] & occupancy) > 0)//if position found
			{
				type = i;
				break;
			}

		///Get the attack table for the given position and given piece
		uint64_t specificAttack = 0;
		int square = shiftAndPosition-1;
		bool modified = false;
		uint64_t allOcc = (refFriendly ^ refEnemy);

		switch (type){
			//pawn should be & with diagonal and forward attack since diagonal is enemy capture mvments
			//do two forward attack seperately becuase it is inexpensive operation
			//pawn currently not working due to modifications
			case 0: {//specificAttack = wpawnAtt[square]; 
				specificAttack ^= wpawnForwardAtt[square];
				
				specificAttack &= refEnemy;
				break;}//w pawn
			case 1: case 7: {specificAttack = knightAtt[square]; break;}//knight for both sides and likewise for bishop, rook, queen and king
			///bishop
			case 2: case 8: { //specificAttack = bishopOccMask[square]; 
				specificAttack = getBishopAttack(square, allOcc);
				modified = true; break;}
			///rook
			case 3: case 9: { //specificAttack = rookOccMask[square]; 
				specificAttack = getRookAttack(square, allOcc);		
				modified = true; break;}
			case 4: case 10: {specificAttack = queenOccMask[square]; 
				specificAttack = (getRookAttack(square, allOcc) ^ getBishopAttack(square, allOcc));
				modified = true; break;}
			case 5: case 11: {specificAttack = kingAtt[square]; break;}
			case 6: { //specificAttack = bpawnAtt[square]; 
				break;}//b pawn, unique
			default: {specificAttack = -10; //-10 is arbitrary here
				printf("Error, movement does not align with the current board representation; check method getMoves/pruning search");}
		}
		
		//sliding piece for bishop/rook shon here, dumb7fill methods used. Note that attack for sliding pieces are attack masks here
		/*if (type == 2 || type ==8)
		{
			//char** biInfo = getBishopSlide(allOcc);
			specificAttack = getBishopAttack(square, allOcc);
			modified = true;
		}
		//rook shown here
		else if (type ==3 || type ==9)
		{
			//occupied
			//char** rooked = getRookSlide(allOcc);
			specificAttack = getRookAttack(square, allOcc);
			modified = true;
		}*/
		//More relevant to non sliding piece, although sliding pieces est necesse as shown through tests
		specificAttack &= ~refFriendly;
		
		printf("\nspeciic attack shown below\n");	
		printBoard(specificAttack);
		///Now iterate through the attack table positions for search
		for (int i=0; i < __builtin_popcount(specificAttack); i++)
		{
			int shifter = ffs(specificAttack);
			uint64_t mvment;
			
			///set the occupation bit and update the board to represent this destination movement
			mvment |= 1<<shifter;

			Move* thisMove = new Move(Color, type, occupancy, mvment);
			mvlist.push_back(thisMove);

			///toggle off specificAttacks shifted one bit to move to next attack
			specificAttack ^= mvment;
		}
		///toggle off least significant bit, and update rm, resetting index
		friendly ^= occupancy;
		exists = ffs(friendly);
		printf("\n updated friendly below\n");
		printBoard(friendly);
		if (COUNT > bound)
			breakPoint();
		//return mvlist;
////////	
//return
///////
		COUNT ++;
	}	
	return mvlist;
}

/*Private maintenance methods*/

///check if a given rank or file is in range to be computed
bool Board::inRange(int n){ return (!(n<0 || n>7)); }

///Used to precompute attack tables for nonsliding pieces; don't need to play in pointer space here; a perfect hash may be given by ffs() on a bitstring with given piece position
void Board::setAttack(int rankoffset[], int fileoffset[], uint64_t destination[], int size){
	int attackIndex =0;
	for (int i =0; i<8; i++)
	{
		for (int j =0;j< 8; j++)
		{
			//get files one and two to left/right
			//get ranks one and two to left/right
			uint64_t attacks[8];
			int index=0;
			//go through the eight positions
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
			if(index > 0)
			{
				allAttack = attacks[0];
				for (int k = 1; k<index; k++)
					allAttack ^= attacks[k];
			}
			//attack table for knight has been computed. now add this to an array to be hashed from for position
			destination[attackIndex] = allAttack;
			//printBoard(allAttack);
			attackIndex++;//go 0-63		
		}
	}
}

void Board::setKnightAttack(){
	//get files one and two to left/right
	//get ranks one and two to left/right
	int tmp1[8]={1, 1, -1, -1, 2, 2, -2, -2};
	int tmp2[8]={2, -2, 2, -2, 1, -1, 1, -1};
	setAttack(tmp1, tmp2, knightAtt, 8);
}

///Similar to the KnightAttack, but it is importatn to note the first rank and eighth rank is not occupiable by white and black respectively
void Board::setPawnAttack(){
	//Set White
	int fileForward[1] = {0};
	int wrankForward[1] = {1};
	int brankForward[1] = {-1};
	
	int fileDiagonal[2] = {1, -1};
	int wrankDiagonal[2] = {1, 1};
	int brankDiagonal[2] = {-1,-1};
			
	setAttack(wrankDiagonal, fileDiagonal, wpawnDiagonalAtt, 2);
	setAttack(brankDiagonal, fileDiagonal, bpawnDiagonalAtt, 2);
	setAttack(wrankForward, fileForward, wpawnForwardAtt, 2);
	setAttack(brankForward, fileForward, bpawnForwardAtt, 2);
}

void Board::setKingAttack(){
	int rankoffset[8] = {0,0, 1,1,1,-1,-1,-1};
	int filesoffset[8]= {1,-1,0,1,-1,0,1,-1};
	setAttack(rankoffset, filesoffset, kingAtt, 8);
}

bool Board::inRangeMagic(int n) { return (!(n==0 || n==7)); }

int Board::max(int i, int j){ return ((i <j) ? j : i); }

void Board::setBishopAttack(){
	//First set the occupancy masks
	int counter =0;
	for (int i =0; i < 8; i++)
		for (int j=0; j<8;j++)
		{
			vector<uint64_t> att;//all sep attack positions of bishop, to be XORed later
			for (int k = 1; k<8; k++)//here 8 should be adjusted due to edge pieces being inconsistently accounted for
			{
				//i is rank, j is file
				vector <uint64_t> values;
				//switch j and i
				if (inRange(i+k) && inRange(j+k))
					values.push_back(ranks[i+k] & files[j+k]);
				if (inRange(i-k) && inRange(j-k))
					values.push_back(ranks[i-k] & files[j-k]);
				if (inRange(i-k) && inRange(j+k))
					values.push_back(ranks[i-k] & files[j+k]);
				if (inRange(i+k) && inRange(j-k))
					values.push_back(ranks[i+k] & files[j-k]);
				uint64_t allAttack;
				if (values.size()>0){//for safety but a.s. guranteed to be greater than zero
					allAttack = values[0];
					for(int c = 1; c < values.size(); c++)
						allAttack^=values[c];
				}
				att.push_back(allAttack);
			}
			uint64_t allAttack = att[0];
			for (int k = 1; k<att.size(); k++)
				allAttack ^= att[k];
			bishopOccMask[counter] = allAttack;
			counter ++;
		}
}

//dumb7fill methods (flood fill based), see chessprogrammingwiki
uint64_t Board::floodRight(uint64_t slider, uint64_t blocker, int shift){
	uint64_t border = 0xFFFFFFFFFFFFFFFF;
	//blocker is the ~friendly
	uint64_t flood = slider;
	blocker &= border;
	flood |= slider = (slider >> shift) & blocker;
	flood |= slider = (slider >> shift) & blocker;
	flood |= slider = (slider >> shift) & blocker;
	flood |= slider = (slider >> shift) & blocker;
	flood |= slider = (slider >> shift) & blocker;
	flood |= (slider >> shift) & blocker;
	return (flood >>shift) & border;
}

uint64_t Board::floodLeft(uint64_t slider, uint64_t blocker, int shift){
        uint64_t border = 0xFFFFFFFFFFFFFFFF;
        //blocker is the ~friendly
        uint64_t flood = slider;
        blocker &= border;
        flood |= slider = (slider >> shift) & blocker;
        flood |= slider = (slider >> shift) & blocker;
        flood |= slider = (slider >> shift) & blocker;
        flood |= slider = (slider >> shift) & blocker;
        flood |= slider = (slider >> shift) & blocker;
        flood |= (slider >> shift) & blocker;
        return (flood >>shift) & border;
}

uint64_t Board::getRookAttack(int sq, uint64_t blocker){
        const int NS_FILL = 8;
        const int EW_FILL = 1;
	return floodRight(flags[sq], ~blocker, NS_FILL)
		| floodRight(flags[sq], ~blocker, EW_FILL)
		| floodLeft(flags[sq], ~blocker, NS_FILL)
		| floodLeft(flags[sq], ~blocker, EW_FILL);

}

uint64_t Board::getBishopAttack(int sq, uint64_t blocker){
        const int NE_SW_FILL = 9;
        const int NW_SE_FILL = 7;
        return floodRight(flags[sq], ~blocker, NE_SW_FILL )
                | floodRight(flags[sq], ~blocker, NW_SE_FILL)
                | floodLeft(flags[sq], ~blocker, NE_SW_FILL)
                | floodLeft(flags[sq], ~blocker, NW_SE_FILL);
}

void Board::setRookAttack(){
	//First set the occupancy masks
	int counter=0;
	for (int i=0; i < 8; i++)
		for (int j =0; j< 8; j++)
		{
			//i is file, j is rank
			uint64_t attack = (ranks[i] ^ files[j]);
			
			//get rid of edges during magic generation
			rookOccMask[counter] = attack;
			counter++;
		}	
	//initialize magic numbers
	//set the magic
	//go through and set in a 2d array
	//getRookAttack(ffs(occ), ~friendly);
}

void Board::setQueenAttack(){ for (int i =0; i < 64; i++) queenOccMask[i] = (bishopOccMask[i]^rookOccMask[i]); }

uint64_t Board::reverse(register uint64_t x){
	//http://stackoverflow.com/questions/21507678/c-64bit-byte-swap-endian
	x = (x & 0x00000000FFFFFFFF) << 32 | (x & 0xFFFFFFFF00000000) >> 32;
	x = (x & 0x0000FFFF0000FFFF) << 16 | (x & 0xFFFF0000FFFF0000) >> 16;
	x = (x & 0x00FF00FF00FF00FF) << 8  | (x & 0xFF00FF00FF00FF00) >> 8;
	return x;
}

void Board::printBoard(uint64_t board)
{
	printf("\n");
	bitset<64>b(board);
	string tmp = b.to_string();
	
	//printf("%s is string\n", tmp.c_str());
	
	vector<vector<char> > total;	

	int count = 0;
	for (int i =0; i < 8; i ++)
	{
		vector <char> invert;
		printf("\n");
		for (int j=0;j <8;j++)
		{
			printf("%c ", tmp[count]);		

			invert.push_back(tmp[count]);
			count++;
		}
		total.push_back(invert);
	}	
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
			for (int i=0; i < 12; i++)
				if((boards[i] & flags[count]) > 0)
				{
					bnum =i;
					break;
				}			 
			switch (bnum){
				case 0: { printf("\u2659 "); break; }
				case 1:	{ printf("\u2658 "); break; }
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

void Board::setup(){
	//set flags for which to use during printout 
	uint64_t changing = 0x0000000000000001; 
	for (int i =0; i<64;i++)
	{
		if(i ==0) 
			flags[i] = 1;
		if(i > 0)
		{
			changing<<=1;
			flags[i] = changing;//left shift
		}
	//printf("%" PRIu64 "\n", flags[i]); 
	}
	//set initial pieces
	//possibly update later
	woccupation = 0x000000000000FFFF;
	boccupation = 0xFFFF000000000000;

	wpawn = 0x000000000000FF00;
	wknight=0x0000000000000042;
	wbishop=0x0000000000000024;
	wrook = 0x0000000000000081;
	wqueen =0x0000000000000010;
	wking = 0x0000000000000008;

	bpawn = 0x00FF000000000000;
	bknight=0x4200000000000000;
	bbishop=0x2400000000000000;
	brook = 0x8100000000000000;
	bqueen =0x1000000000000000;
	bking = 0x0800000000000000;

	//vertical
	afile = 0x8080808080808080; //8080808080808080;
	bfile = 0x4040404040404040;
	cfile = 0x2020202020202020;
	dfile = 0x1010101010101010;
	efile = 0x0808080808080808;
	ffile = 0x0404040404040404;
	gfile = 0x0202020202020202;
	hfile = 0x0101010101010101;

	files[7]=afile;
	files[6]=bfile;
	files[5]=cfile;
	files[4]=dfile;
	files[3]=efile;
	files[2]=ffile;
	files[1]=gfile;
	files[0]=hfile;
	
	//horizontal
	rank1 = 0x00000000000000FF;
	rank2 = 0x000000000000FF00;
	rank3 = 0x0000000000FF0000;
	rank4 = 0x00000000FF000000;
	rank5 = 0x000000FF00000000;
	rank6 = 0x0000FF0000000000;
	rank7 = 0x00FF000000000000;
	rank8 = 0xFF00000000000000;

	ranks[0] = rank1;
	ranks[1] = rank2;
	ranks[2] = rank3;
	ranks[3] = rank4;
	ranks[4] = rank5;
	ranks[5] = rank6;
	ranks[6] = rank7;
	ranks[7] = rank8;

	boards.push_back(wpawn);
	boards.push_back(wknight);
	boards.push_back(wbishop);
	boards.push_back(wrook);
	boards.push_back(wqueen);
	boards.push_back(wking);
	boards.push_back(bpawn);
	boards.push_back(bknight);
	boards.push_back(bbishop);
	boards.push_back(brook);
	boards.push_back(bqueen);
	boards.push_back(bking);
	boards.push_back(woccupation);
	boards.push_back(boccupation);
	
	setKnightAttack();
	setPawnAttack();
	setKingAttack();
	setBishopAttack();
	setRookAttack();
	setQueenAttack();
}

void Board::printString(uint64_t board){
	bitset<64>b (board);
	string tmp = b.to_string();
	printf("%s is string\n", tmp.c_str());
}

void Board::breakPoint(){
	printf("\nabort\n");
	abort();
}
