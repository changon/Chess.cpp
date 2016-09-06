/*
	TODO
			position - play from position provided via fenstring
			search parameters in go
			algebraic notation translation to board
			
*/

#include "Play.h"
#include "Board.h"
#include <iostream>
#include <stdio.h>
#include <string>

using namespace std;

int main()
{
	string name = "Sabor";
	string version = "0.1";
	string author = "Raphael Kim";
	
	Board board;
	cout << name << " " << version << " by " << author <<endl;
	
	string inp;

	/*while (inp != "quit"){
		getline(cin, inp);
		if (inp == "uci"){
			cout << "id name " << name << " " << version <<endl;
			cout << "id author "<<author << endl;
			cout << "option name Threads type spin default " << DEFAULT_THREADS << " min " << MIN_THREADS << " max " << MAX_THREADS << endl;
			cout << "option name Hash type spin default " << DEFAULT_HASH << " min "<<MIN_HASH<< " max " << MAX_HASH<<endl;
			cout << "uciok" <<endl;
		}
		else if (inp == "isready") cout << "isready"<<endl;
		else if (inp == "ucinewgame") cout<<"ucinewgame"<<endl;
		else if (inp == "register") cout<<"register later"<<endl;
		else if (inp == "position") cout <<"FEN STRING to stream"<<endl;
		else if (inp == "go") cout << "GO" <<endl;
	}*/
	board.play(9);
}
