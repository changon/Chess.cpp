CS 410: Raphael Kim, final project

This is the current progress of writing a chess engine in C++ that is based on the bitboard data structure as opposed to the 8x8 array approach.

ABOUT, STRUGGLES, CURRENT PROGRESS:
The implementation is not functioning. An ideal checkpoint would be having the computer play against itself. However I became rather caught up in the move generation trying to learn about magic bitboards. 

To summarize magic bitboards in short, chess is sometimes programmed using precomputed attack tables, where for each move, you are able to obtain the specific attack table (where a piece can move) based on the square occupied and occupancy. 

This can be obtained with a injective (unique codomain for every domain) function or perfect hashing. While for nonsliding pieces this is trivial, there are many more permutations possible for sliding piecesgenerations due to blocking midway of sliding (rooks, bishops, queens). Magic bitboards are simply a way to create this perfect hashing to obtain the attack table for a given occupancy board in constant time.

Because I did not finish the attack table generation, I was unable to verify the alphabeta pruning and general functioning framework. More, I decided to focus on understanding magic bitboards. The general framework is laid out however. I inserted breakpoints at the move generation for the extent of where I got up to. I especially had trouble precomputing these attack tables effeciently as I did not enjoy the string operations so I deleted it and contemplated for a while on bitwise options. 

FUTURE WORK:
Future work includes utilizing transposition tables to speed up alpha beta pruning and implement iterative deepening during the beginning as that is very profitable in the beginning of chess games. In addition I would like to complete the Evaluation function for this chess engine (a complex, trivial, and learned). 

Learned referring to finally completing training a neural network evaluation function. In this scenario, the subsequent killer heuristics, quiet moves, and other moves based on a non-learned evaluation function are not as relevant and move ordering is only important (in C++ that is).

UCI:
I also spent a fair amount of time looking into making the chess engine UCI compliant. I however stopped partway. The GUI I experimented, arena, is included in here. For Mac or Linux users, it should be run via a wrapper such as Wineskin.

USAGE:
There is a primitive makefile in which I simply compile the necessary files. 
To compile, run 'make install'.

Afterwards execute uci_play using './uci_play' 

What you are able to see is only the trivial attack tables for non sliding pieces.
