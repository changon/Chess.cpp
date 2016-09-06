#include "Move.h"
#include <string>
#include <inttypes.h>
#include <bitset>
#include <vector>

using namespace std;

Move::Move(std::string color, int modifier, uint64_t src, uint64_t destination){
	this->COLOR = color;
	this->type = modifier;
	this->src = src;
	this->destination = destination;
	
	//set the other parameters
	//such as enemy piece uint64_t to add/sub with
}

/*Getters*/
uint64_t Move::getSource() { return this->src; }

uint64_t Move::getDestination() { return this->destination; }

string Move::getColor() { return this->COLOR; }

int Move::getType() { return this->type; }

int Move::getOldType() { return this->oldType; }

void Move::print(){
	printf("%s\n", COLOR.c_str());
	printf("type is %d\n", type);
	pp(src);
	pp(destination);
}

void Move::setOldType(int old) { this->oldType = old; }

void Move::pp(uint64_t printee)
{
	bitset<64>b(printee);
	string tmp = b.to_string();

	vector<vector<char> > total;	

	int count = 63;
	for (int i =0; i < 8; i ++)
	{
		vector <char> invert;
		for (int j=0;j <8;j++)
		{
			invert.push_back(tmp[count]);
			count--;
		}
		total.push_back(invert);
	}	
	for (int i=7; i >-1; i--)
		for (int j=0; j<8;j++)
			printf("%c ", total[i][j]);
	printf("\n");
}
