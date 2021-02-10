#include "utils.h"
int main()
{
	board brd=initializeBoard(5);
	showInstructions(brd);
	int winner = playGame(brd);
	saveWinnerInFile(winner);
	freeBoard(brd);
}
