#include "utils.h"
#include<stdio.h>
#include<stdlib.h>

board initializeBoard(int size)
{
	board brd;
	brd.size=size;
	brd.M=malloc(sizeof(char*)*size);
	for(int i=0;i<size;i++)
		brd.M[i]=malloc(sizeof(char)*size);

	for(int i=0;i<brd.size;i++)
		for(int j=0;j<brd.size;j++)
			brd.M[i][j]=' ';
	return brd;
}

void freeBoard(board brd)
{
	for(int i=0;i<brd.size;i++)
		free(brd.M[i]);
	free(brd.M);
}

void saveWinnerInFile(int winner)
{
	printf("GAME FINISHED\n");
	char filename[30] = "winner.txt";

	// TODO salvati winner 9PLAYER1, PLAYER2, DRAW) in fisierul filename

}

void showInstructions(board brd)
{
	printf("\nTic Tac Toe\n");
	printf("insert coordonates for a %dx%d matrix starting at 0,0\n",brd.size,brd.size);
	// printf("-\t-\t-\t-\t-\t-\t-\t-\t-\t-\n");
	// printf("   -------------\n");
	printf("\n");
	printf("    0,0 | 0,1 \n");
	printf("   -----------\n");
	printf("    1,0 | 1,1 \n");
}

void showBoard(board brd,  int round)
{
	printf("\nTic Tac Toe - round %d\n\n", round);
    printf("Player 1 (X)  -  Player 2 (O)\n");

	printf("  %c ", brd.M[0][0]);
	for(int j=1;j<brd.size;j++)
		printf("| %c ", brd.M[0][j]);
	printf("\n");
	for(int i=1;i<brd.size;i++)
	{
		printf(" ");
		for(int j=0;j<brd.size;j++)
			printf("----");
		printf("\n");

		printf("  %c ", brd.M[i][0]);
		for(int j=1;j<brd.size;j++)
			printf("| %c ", brd.M[i][j]);
		printf("\n");

	}
	// printf("\t %c | %c | %c \n", brd[0][0], brd[0][1], brd[0][2]);
	// printf("\t--------------\n");
	// printf("\t %c | %c | %c \n", brd[1][0], brd[1][1], brd[1][2]);
	// printf("\t--------------\n");
	// printf("\t %c | %c | %c \n\n", brd[2][0], brd[2][1], brd[2][2]);
}

int playGame(board brd)
{
	int player = PLAYER1, winner = PLAYING, choice, round = 1;

	char mark=MOVEX;
	// showBoard(brd,round-1);
	char *cell;
	while(checkWin(brd,round,mark)==PLAYING)
	{
		// TOT CLEAR SCREN
		showBoard(brd,round);
		round++;
		do
		{
			int x,y;
			scanf("%d %d",&x,&y);
			cell=getCell(brd,x,y);
		}
		while(cell==NULL || !checkCellIsEmpty(cell));
		fillCell(cell,mark);
		if(checkWin(brd,round,mark)==1)
		{
			printf("%d\n",checkWin(brd,round,mark) );
			showBoard(brd,round);
			printf("Player%d won\n",player );
			return player;
		}
		// printf("|%c|%c|\n", ((mark==MOVEX)*MOVEO),((mark==MOVEO)*MOVEX));
		mark=((mark==MOVEX)*MOVEO)+((mark==MOVEO)*MOVEX);
		player=(~(player-1))+1;
	}
	showBoard(brd,round);
	printf("Draw\n");
	return 0;

	// TODO implement game mechanism
	// use the methods:
	// getCell, cellIsEmpty, fillCell, checkWin, showBoard
	// read the desired move (cell number from 1 to 9) from the player
	// if the move can be done, complete it and check the state of the game

    return winner;
}

char* getCell(board brd, int x,int y)
{
	if(x>=brd.size || x>=brd.size)
		return NULL;
	return &(brd.M[x][y]);
}

int checkCellIsEmpty(char* cell)
{
	return cell[0]==' ';
}

void fillCell(char *cell, char mark)
{
	cell[0]=mark;
}

// -1 still playing
// 0 draw
// 1 player 1 winner
// 2 player 2 winner
int checkWin(board brd,int round, char mark)
{
    // TODO
	// check if a line was completed
	// else check if all cells were completed => draw

	for(int i=0;i<brd.size;i++)
		for(int j=0;j<brd.size;j++)
			if(brd.M[i][j]!=mark)
				break;
			else if(j==brd.size-1)
				return 1;

	for(int j=0;j<brd.size;j++)
		for(int i=0;i<brd.size;i++)
			if(brd.M[i][j]!=mark)
				break;
			else if(i==brd.size-1)
				return 1;

	for(int j=0;j<brd.size;j++)
		if(brd.M[j][j]!=mark)
			break;
		else if(j==brd.size-1)
			return 1;

	for(int j=0;j<brd.size;j++)
		if(brd.M[j][brd.size-j-1]!=mark)
			break;
		else if(j==brd.size-1)
			return 1;

	if(round>brd.size*brd.size)
		return 0;
	return  PLAYING;
}
