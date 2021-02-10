#define SIZE 3

# define MOVEX 'X'
# define MOVEO 'O'

typedef struct
{
	char **M;
	int size;
} board;

enum State {PLAYING = -1, DRAW = 0, PLAYER1 = 1, PLAYER2 = 2};

board initializeBoard(int size);
void freeBoard(board brd);
void saveWinnerInFile(int winner);
void showInstructions(board brd);
void showBoard(board brd,  int round);
int playGame(board brd);
char* getCell(board brd, int x,int y);
int checkCellIsEmpty(char* cell);
void fillCell(char *cell, char mark);
int checkWin(board brd,int round, char mark);
