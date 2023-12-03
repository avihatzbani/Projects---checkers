


#include "checkersHeader.h"


/*That function gets a cheker boardand a square
The function check all the posibble moves for the player in the square*/
SingleSourceMovesTree* FindSingleSourceMoves(Board board, checkersPos* src) {


	SingleSourceMovesTree* Tree;
	Tree = (SingleSourceMovesTree*)malloc(sizeof(SingleSourceMovesTree));
	int row = changeCharToNum(src->row);
	int col = changeCharToNum(src->col);
	if (board[row][col] == ' ')
		return NULL;
	else if (board[row][col] == 'T')//If there is'nt a player in the current square
		Tree->source = helperBuildTreeForPlayerT(board, row, col, NO_CAPTURES);
	else
		Tree->source = helperBuildTreeForPlayerB(board, row, col, NO_CAPTURES);

	return Tree;
}



// Helper function that check all the posibble moves of player 'T'
SingleSourceMovesTreeNode* helperBuildTreeForPlayerT(Board board, int row, int col, int captures)
{

	SingleSourceMovesTreeNode* root = createNewTreeNode(board, captures, row, col);

	//If the player don't have a legal move
	if (board[row + 1][col + 1] == 'T' && board[row + 1][col - 1] == 'T')
		return root;
	if ((board[row + 1][col + 1] == 'T' && col == 0) || (board[row + 1][col - 1] == 'T' && col == 7))
		return root;


	//Move one step to the right side(without any capture), if it's posibble
	if (board[row + 1][col + 1] == ' ' && col != 7 && root->total_captures_so_far == NO_CAPTURES)
		root->next_move[RIGHT] = createNewTreeNode(board, NO_CAPTURES, row + 1, col + 1);
	

	//Move to the right side with captures(if it's posibble)
	if (board[row + 1][col + 1] == 'B' && (row < 6) && (col < 6) && board[row + 2][col + 2] == ' ')
		root->next_move[RIGHT] = helperBuildTreeForPlayerT(board, row + 2, col + 2, captures + 1);
	

	//Move one step to the left side(without any capture), if it's posibble
	if (board[row + 1][col - 1] == ' ' && col != 0 && root->total_captures_so_far == NO_CAPTURES)
		root->next_move[LEFT] = createNewTreeNode(board, NO_CAPTURES, row + 1, col - 1);

	//Move to the left side with captures(if it's posibble)
	if (board[row + 1][col - 1] == 'B' && (row < 6) && (col > 1) && board[row + 2][col - 2] == ' ')
		root->next_move[LEFT] = helperBuildTreeForPlayerT(board, row + 2, col - 2, captures + 1);

	return root;

}

// Helper function that check all the posibble moves of player 'B'
SingleSourceMovesTreeNode* helperBuildTreeForPlayerB(Board board, int row, int col, int captures)
{

	SingleSourceMovesTreeNode* root = createNewTreeNode(board, captures, row, col);

	//If the player don't have a legal move

	if (board[row - 1][col + 1] == 'B' && board[row - 1][col - 1] == 'B')
		return root;
	if ((board[row - 1][col + 1] == 'B' && col == 0) || (board[row - 1][col - 1] == 'B' && col == 7))
		return root;
	
	
	//Move one step to the right side(without any capture), if it's posibble
	if (board[row - 1][col + 1] == ' ' && col != 7 && root->total_captures_so_far == NO_CAPTURES)
		root->next_move[RIGHT] = createNewTreeNode(board, NO_CAPTURES, row - 1, col + 1);
	
	//Move to the right side with captures(if it's posibble)
	if (board[row - 1][col + 1] == 'T' && (row > 1) && (col < 6) && board[row - 2][col + 2] == ' ')
		root->next_move[RIGHT] = helperBuildTreeForPlayerB(board, row - 2, col + 2, captures + 1);
	

	//Move one step to the left side(without any capture), if it's posibble
	if (board[row - 1][col - 1] == ' ' && col != 0 && root->total_captures_so_far == NO_CAPTURES)
		root->next_move[LEFT] = createNewTreeNode(board, NO_CAPTURES, row - 1, col - 1);

	

	//Move to the left side with captures(if it's posibble)
	if (board[row - 1][col - 1] == 'T' && (row > 1) && (col > 1) && board[row - 2][col - 2] == ' ')
		root->next_move[LEFT] = helperBuildTreeForPlayerB(board, row - 2, col - 2, captures + 1);
	
	return root;
}



//Function that create new tree node
SingleSourceMovesTreeNode* createNewTreeNode(Board board, unsigned int total_captures_so_far, int row, int col) 
{

	SingleSourceMovesTreeNode* res = ((SingleSourceMovesTreeNode*)malloc(sizeof(SingleSourceMovesTreeNode)));
	copyBoardGame(res->board, board);
	res->pos = (checkersPos*)malloc(sizeof(checkersPos));
	res->pos->col = col + '0';
	res->pos->row = row + '0';
	res->total_captures_so_far = total_captures_so_far;
	res->next_move[LEFT] = NULL;  
	res->next_move[RIGHT] = NULL; 

	return res;
}


//function to print the tree's variables in order
void printTreeInorder(SingleSourceMovesTree* t)
{
	print_in_order_rec(t->source);
}

//recursive helper function to print the tree's variables in order
void print_in_order_rec(SingleSourceMovesTreeNode* n)
{
	if (n == NULL)
		return;
	print_in_order_rec(n->next_move[LEFT]);
	printf("tree col: %c ", n->pos->col);
	printf("tree row: %c ", n->pos->row);
	printf("tree captures: %d ", n->total_captures_so_far);
	print_in_order_rec(n->next_move[RIGHT]);
}

//This function copy the current board to new one 
void copyBoardGame(Board resBoard, Board board)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			resBoard[i][j] = board[i][j];
		}
	}

}

//This function find the maximum captures the player did
int findMaxCaptures(SingleSourceMovesTreeNode* t)
{
	if (t == NULL)
		return NO_CAPTURES;

	int leftMax = findMaxCaptures(t->next_move[LEFT]);
	int rightMax = findMaxCaptures(t->next_move[RIGHT]);

	int maxNumber = max(leftMax, rightMax);

	return max(maxNumber, t->total_captures_so_far);

}




//This function gets a tree and free its memory
void freeTree(SingleSourceMovesTree* tr)
{
	freeTreeHelper(tr->source);
	tr->source = NULL;
}

//Recursive helper function that free tree's memory
void freeTreeHelper(SingleSourceMovesTreeNode* root)
{
	if (root == NULL)
		return;
	else
	{
		freeTreeHelper(root->next_move[LEFT]);
		freeTreeHelper(root->next_move[RIGHT]);
		free(root);
	}
}





//Function to chech allocation
void checkMemoryAllocation(void* ptr)
{
	if (ptr == NULL)
	{
		puts("Memory allocation error");
		exit(1);
	}
}

//Function that gets a char and cange it to integer
int changeCharToNum(char ch)
{
	return (ch - '0');

}
