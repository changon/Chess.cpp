/*Without at learned evaluation function, move will be the parent class of kill moves, quiet moves, etc.*/

#ifndef MOVE_H
#define MOVE_H

#include <string>
#include <inttypes.h>

class Move{
	public:
		Move(std::string color, int type, uint64_t source, uint64_t destination);
		void print();
		void pp(uint64_t);//for binary printout of chess board
		void setOldType(int old);
		/*Getters*/
		uint64_t getSource();
		uint64_t getDestination();
		std::string getColor();
		int getType();
		int getOldType();
	private:
		/*Holds the board to modify of 0-12 in boards[] array of Board*/
		int type;
		uint64_t src, destination;
		std::string COLOR;
		int enemy;
		int oldType; //type of piece that is being replaced		
};

#endif
