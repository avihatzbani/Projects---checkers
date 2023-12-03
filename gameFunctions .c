


#include "checkersHeader.h"

//Function that give the start board of the game
void startBoard(Board board)
{
	int row, col;


	// fill the T's squares
	for (row = 0; row < 3; row++) 
	{
		for (col = 0; col < 8; col++)
		{
			if ((row + col) % 2 == 1) 
				board[row][col] = 'T';
			else
				board[row][col] = ' ';
		}
	}

	// fill the B's squares
	for (row = 5; row < 8; row++)
	{
		for (col = 0; col < 8; col++) 
		{
			if ((row + col) % 2 == 1) 
				board[row][col] = 'B';
			else
				board[row][col] = ' ';
		}
	}
	
	// fill the empty squares withe blankspace
	for (row = 3; row < 5; row++)
	{
		for (col = 0; col < 8; col++)
			board[row][col] = ' ';
		
	}

}


//That function gets a board ant a player and plays the best move of the player from all the player's squares
void Turn(Board board, Player player)
{
	multipleSourceMovesList* movesList;
	MultipleSourceMovesListCell* currCell, * optimalMovesCell;
	checkersPos* start, * end;
	
	// Find all possible moves for the player
	movesList = FindAllPossiblePlayerMoves(board, player);

	currCell = movesList->head;
	optimalMovesCell = currCell;
	char row, col;
	row = optimalMovesCell->single_source_moves_list->head->position->row;
	col = optimalMovesCell->single_source_moves_list->head->position->col;

	while (currCell != NULL)
	{
		// If the current optimal move has no more captures, update the optimal move to the next move
		if (optimalMovesCell->single_source_moves_list->head->next == NULL)
		{
			optimalMovesCell = currCell->next;
			currCell = currCell->next;
			row = optimalMovesCell->single_source_moves_list->head->position->row;
			col = optimalMovesCell->single_source_moves_list->head->position->col;
		}

		// If the current move has more captures than the optimal move, update the optimal move
		if (optimalMovesCell->single_source_moves_list->tail->captures < currCell->single_source_moves_list->tail->captures)
		{
			optimalMovesCell = currCell;
			row = optimalMovesCell->single_source_moves_list->head->position->row;
			col = optimalMovesCell->single_source_moves_list->head->position->col;
		}
		
		// If the current move has the same number of captures as the optimal move, check the positions
		else if (optimalMovesCell->single_source_moves_list->tail->captures == currCell->single_source_moves_list->tail->captures)
		{
			if (currCell->single_source_moves_list->head->next != NULL)
			{           
				// If the current move's position is more favorable than the optimal move's position, update the optimal move
				if ((player == 'T' && row < currCell->single_source_moves_list->head->position->row) ||	(player != 'T' && row > currCell->single_source_moves_list->head->position->row))
				{
					optimalMovesCell = currCell;
					row = optimalMovesCell->single_source_moves_list->head->position->row;
					col = optimalMovesCell->single_source_moves_list->head->position->col;
				}
				// If the current move's row is the same as the optimal move's row, check the col
				else if (row == currCell->single_source_moves_list->head->position->row)
				{
					// If the current move's column is more favorable than the optimal move's column, update the optimal move
					if ((player == 'T' && col< currCell->single_source_moves_list->head->position->col) || (player != 'T' && col > currCell->single_source_moves_list->head->position->col))
					{
						optimalMovesCell = currCell;
						row = optimalMovesCell->single_source_moves_list->head->position->row;
						col = optimalMovesCell->single_source_moves_list->head->position->col;
					}
				}
			}
		}
		currCell = currCell->next;
	}

	BoardAfterMove(board, optimalMovesCell->single_source_moves_list);//making new Board

	start = optimalMovesCell->single_source_moves_list->head->position;
	end = optimalMovesCell->single_source_moves_list->tail->position;

	printf("%c's Turn:\n", player);
	printf("\n%c%c->%c%c\n", start->row - '0' + 'A', start->col + 1, end->row - '0' + 'A', end->col + 1);

}



//That function update the board accordance to the current move 
void BoardAfterMove(Board board, SingleSourceMovesList* lst)
{
	SingleSourceMovesListCell* CurrSingleListNode = lst->head;
	

	int startRow = changeCharToNum(lst->head->position->row);
	int startCol = changeCharToNum(lst->head->position->col);
	int endRow = changeCharToNum(lst->tail->position->row);
	int endCol = changeCharToNum(lst->tail->position->col);
	char player = board[startRow][startCol];

	if (lst->tail->captures == NO_CAPTURES) // no captures - no needed to delete opposite player
	{
		board[startRow][startCol] = ' ';
		board[endRow][endCol] = player;
	}
	else
	{
		while (CurrSingleListNode->next != NULL)
		{
			if (player == 'B') 
			{
				board[startRow][startCol] = ' '; // player has moved
				int midRow = (changeCharToNum(CurrSingleListNode->position->row) - 1); //taking one less line
				int midCol = (changeCharToNum(CurrSingleListNode->position->col) + changeCharToNum(CurrSingleListNode->next->position->col)) / 2; // the avg of both cols, end and start
				board[midRow][midCol] = ' '; // deleting the opposite player
			}
			if (player == 'T') 
			{
				board[startRow][startCol] = ' '; // player has moved
				int midRow = (changeCharToNum(CurrSingleListNode->position->row) + 1); //taking one more line
				int midCol = (changeCharToNum(CurrSingleListNode->position->col) + changeCharToNum(CurrSingleListNode->next->position->col)) / 2;  // the avg of both cols, end and start
				board[midRow][midCol] = ' '; // deleting the opposite player
			}
			CurrSingleListNode = CurrSingleListNode->next; // keep deleting all captured opposite players
		}
		board[endRow][endCol] = player; // put player in final move square
	}


}




//This function play the checker game
void playGame(Board board, Player starting_player)
{
	Player player = starting_player, theWinner, maxCapturesPlayer;
	int numPlayersOfT = 0, numPlayersOfB = 0;
	int MovesTcounter = 1, MovesBcounter = 1;
	int maxCaptures = 0;
	bool foundWinner = false;
	char oppPlayer;
	int numPlayersBeforMove;
	int numPlayersAfterMove;
	printBoard(board);
	while (!playerGetToLastRow(board, player) && !foundWinner)
	{
		if (player == 'T')
			 oppPlayer = 'B';
		else
			 oppPlayer = 'T';
		numPlayersBeforMove = playersCounter(board, oppPlayer);
		Turn(board, player);//playing player's turn
		printBoard(board);
		numPlayersAfterMove = playersCounter(board, oppPlayer);//count num of the opposite player

		if (maxCaptures < (numPlayersBeforMove - numPlayersAfterMove))
		{
			maxCapturesPlayer = player;
			maxCaptures = numPlayersBeforMove - numPlayersAfterMove;
		}
		

		//if someone wins:
		if (playerGetToLastRow(board, player))
		{
			theWinner = player;
			foundWinner = true;
		}

		if (player == 'T') 
		{
			if (numPlayersAfterMove == NO_PLAYERS)
			{
				theWinner = oppPlayer;
				foundWinner = true;
			}
			else
			{
				MovesBcounter++;
				player = 'B';
			}
		}
		else 
		{
			if (numPlayersAfterMove == NO_PLAYERS)
			{
				theWinner = oppPlayer;
				foundWinner = true;
			}
			else
			{
				MovesTcounter++;
				player = 'T';
			}
		}
			
	}

	//print the winner
	printf("\n.\n.\n.\n");
	printf("%c Wins\n", theWinner);
	if (theWinner == 'T')
	{
		printf("T performed %d moves\n", MovesTcounter );
	}
	else
		printf("B performed %d moves\n", MovesBcounter );


	printf("%c performed the highest number of captures in a single move - %d", maxCapturesPlayer, maxCaptures);
}



//This function chech is the player get to the last row
bool playerGetToLastRow(Board board, Player player)
{

	if (player == 'B')
	{
		for (int i = 0; i < BOARD_SIZE; i++)
		{
			if (board[FIRST_ROW][i] == 'B')
				return true;
		}
	}
	else
	{
		for (int i = 0; i < BOARD_SIZE; i++)
		{
			if (board[LAST_ROW][i] == 'T')
				return true;
		}
	}
	return false;
}


//This function count the numbers of players each Player have
int playersCounter(Board board, Player player)
{
	int counterPlayer = 0;

	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{

			if (board[i][j] == player)
				counterPlayer++;
		}
	}

	return counterPlayer;
}



//Function to print the cheker's board
void printBoard(Board board)
{

	printLine();
	for (int row = 0; row <= BOARD_SIZE; row++)
	{
		for (int col = 0; col <= BOARD_SIZE; col++)
		{
			if (row == 0)
				printf("|%d", col);
			else if (col == 0)
			{
				if (row == 0)
					printf("+ ");
				else
					printf("|%c", row - 1 + 'A');
			}
			else
				printf("|%c", board[row - 1][col - 1]);
		}
		printf("|\n");
		printLine();
	}
}

//Function to print a line in the board
void printLine()
{
	for (int i = 0; i < 19; i++)
	{
		if (i % 2 == 0)
			printf("+");
		else
			printf("-");
	}
	printf("\n");
}