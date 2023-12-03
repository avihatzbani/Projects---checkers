

#include "checkersHeader.h"



void main()
{
	Board board;
	Player player;
	printf("Welcome to checkers game ! ! !\n");
	startBoard(board);
	printBoard(board);
	printf("\nchoose B or T to start: ");
	scanf("%c", &player);
	if ((player == 'B') || (player == 'T'))
		playGame(board, player);
	else {
		printf("Wrong choice.");
		exit(1);
	}
}

