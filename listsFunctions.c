

#include "checkersHeader.h"


//This function gets a player's tree moves and return the move with the maximum captures
SingleSourceMovesList* FindSingleSourceOptimalMove(SingleSourceMovesTree* moves_tree)
{
	SingleSourceMovesList* list;
	SingleSourceMovesListCell* curr;

	int maxCapture = findMaxCaptures(moves_tree->source);

	//gets the player's position
	char player = moves_tree->source->board[changeCharToNum(moves_tree->source->pos->row)][changeCharToNum(moves_tree->source->pos->col)];

	list = (SingleSourceMovesList*)malloc(sizeof(SingleSourceMovesList));
	checkMemoryAllocation(list);
	makeEmptySingleSourceMovesList(list);

	if (maxCapture == NO_CAPTURES)
	{
		if ( moves_tree->source->next_move[LEFT] != NULL && moves_tree->source->next_move[RIGHT] != NULL)
		{
			if (player == 'B')
			{
				//call left first, B needs to go left as requested
				insertDataToStartSingleSourceMovesList(list, maxCapture, moves_tree->source->next_move[LEFT]->pos);
				insertDataToStartSingleSourceMovesList(list, maxCapture, moves_tree->source->pos);
			}
			else
			{
				//call right first, T needs to go right as requested
				insertDataToStartSingleSourceMovesList(list, maxCapture, moves_tree->source->next_move[RIGHT]->pos);
				insertDataToStartSingleSourceMovesList(list, maxCapture, moves_tree->source->pos);

			}

		}
		//means right = NULL, call only left
		else if (moves_tree->source->next_move[LEFT] != NULL)
		{
			insertDataToStartSingleSourceMovesList(list, maxCapture, moves_tree->source->next_move[LEFT]->pos);
			insertDataToStartSingleSourceMovesList(list, moves_tree->source->total_captures_so_far, moves_tree->source->pos);
		}
		//means left = NULL, call only right 
		else if (moves_tree->source->next_move[RIGHT] != NULL)
		{
			insertDataToStartSingleSourceMovesList(list, maxCapture, moves_tree->source->next_move[RIGHT]->pos);
			insertDataToStartSingleSourceMovesList(list, moves_tree->source->total_captures_so_far, moves_tree->source->pos);
		}
		

	}
	
	//means left & right != NULL, and captures > 0. call recursively untill breaking point
	//find optimal move for each player
	if(player=='B')
		HelperFindSingleSourceOptimalMoveB(moves_tree->source, NO_CAPTURES, list);
	else
		HelperFindSingleSourceOptimalMoveT(moves_tree->source, NO_CAPTURES, list);



	return list;
}


void HelperFindSingleSourceOptimalMoveB(SingleSourceMovesTreeNode* moves_tree, int maxCapture, SingleSourceMovesList* lst)
{

	if (moves_tree == NULL)
		return;
	else
	{
		//call left first because player is B
		HelperFindSingleSourceOptimalMoveB(moves_tree->next_move[LEFT], maxCapture + 1, lst);
		HelperFindSingleSourceOptimalMoveB(moves_tree->next_move[RIGHT], maxCapture + 1, lst);

		if (moves_tree->total_captures_so_far == maxCapture)
		{
			//breaking point
			insertDataToStartSingleSourceMovesList(lst, moves_tree->total_captures_so_far, moves_tree->pos);
			maxCapture--;
		}
	}
		
}

void HelperFindSingleSourceOptimalMoveT(SingleSourceMovesTreeNode* moves_tree, int maxCapture, SingleSourceMovesList* lst)
{
	if (moves_tree == NULL)
		return;
	else
	{
		//call right first because player is T
		HelperFindSingleSourceOptimalMoveT(moves_tree->next_move[RIGHT], maxCapture + 1, lst);
		HelperFindSingleSourceOptimalMoveT(moves_tree->next_move[LEFT], maxCapture + 1, lst);
		
		if (moves_tree->total_captures_so_far == maxCapture)
		{
			//breaking point
			insertDataToStartSingleSourceMovesList(lst, moves_tree->total_captures_so_far, moves_tree->pos);
			maxCapture--;
		}
	}
}


/*This function gets a checker board and a player(B or T).
and returns a list with the best moves of every square with the current player */
multipleSourceMovesList* FindAllPossiblePlayerMoves(Board board, Player player)
{
	SingleSourceMovesTree* Tree;
	SingleSourceMovesList* list;

	checkersPos* pos = (checkersPos*)malloc(sizeof(checkersPos));
	checkMemoryAllocation(pos);
	multipleSourceMovesList* TotalMovesList = (multipleSourceMovesList*)malloc(sizeof(multipleSourceMovesList));
	checkMemoryAllocation(TotalMovesList);
	makeEmptyMultipleSourceMovesList(TotalMovesList);

	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			if (board[i][j] == player)
			{
				pos->row = i + '0';//change to char
				pos->col = j + '0';//change to char
				Tree = FindSingleSourceMoves(board, pos);//creat the tree
				list = FindSingleSourceOptimalMove(Tree);//creat the list
				insertDataToEndMultipleSourceMovesList(TotalMovesList, list);
				freeTree(Tree);
			}
		}
	}

	return TotalMovesList;
}




//SingleSourceMovesList functions:

//Function that initializes a list
void makeEmptySingleSourceMovesList(SingleSourceMovesList* lst)
{
	lst->head = lst->tail = NULL;
}

//Function that insert data to start list
void insertDataToStartSingleSourceMovesList(SingleSourceMovesList* lst, int captures, checkersPos* position)
{
	SingleSourceMovesListCell* newHead;
	newHead = createNewSingleSourceMovesListNode(captures, position, NULL); // creating new node
	insertNodeToStartSingleSourceMovesList(lst, newHead);
}


//Function that create new list node
SingleSourceMovesListCell* createNewSingleSourceMovesListNode(int captures, checkersPos* position, SingleSourceMovesListCell* next)
{
	SingleSourceMovesListCell* res;
	res = (SingleSourceMovesListCell*)malloc(sizeof(SingleSourceMovesListCell));
	res->captures = captures;
	res->position = position;
	res->next = next; // pointing the last pointer to NULL
	return res;
}

//Function that insert node to start list
void insertNodeToStartSingleSourceMovesList(SingleSourceMovesList* lst, SingleSourceMovesListCell* head)
{
	if (isEmptySingleSourceMovesList(lst) == true)
	{
		head->next = NULL;
		lst->head = lst->tail = head;
	}
	else
	{
		head->next = lst->head;
		lst->head = head;
	}
}

//Function that gets a list and check if it's empty or not
bool isEmptySingleSourceMovesList(SingleSourceMovesList* lst)
{
	if (lst->head == NULL)
		return true;
	else
		return false;
}




//multipleSourceMovesList functions:

//Function that initializes a list
void makeEmptyMultipleSourceMovesList(multipleSourceMovesList* lst)
{
	lst->head = lst->tail = NULL;
}

//Function that insert data to end list
void insertDataToEndMultipleSourceMovesList(multipleSourceMovesList* lst, SingleSourceMovesList* singleList)
{
	MultipleSourceMovesListCell* newTail;
	newTail = createNewMultipleSourceMovesListNode(singleList, NULL); 
	insertNodeToEndMultipleSourceMovesList(lst, newTail); 

}

//Function that create new list node
MultipleSourceMovesListCell* createNewMultipleSourceMovesListNode(SingleSourceMovesList* singleList, MultipleSourceMovesListCell* next)
{
	MultipleSourceMovesListCell* res;
	res = (MultipleSourceMovesListCell*)malloc(sizeof(MultipleSourceMovesListCell));
	res->single_source_moves_list = singleList;
	res->next = next;
	return res;
}

//Function that insert node to end list
void insertNodeToEndMultipleSourceMovesList(multipleSourceMovesList* lst, MultipleSourceMovesListCell* tail)
{
	if (isEmptyMultipleSourceMovesList(lst) == true)
		lst->head = lst->tail = tail;
	else
	{
		lst->tail->next = tail; // inserting the new node to the end of the list
		lst->tail = tail; // pointing the tail to the new nod that is in the end of the list
	}
	tail->next = NULL;
}

//Function that gets a list and check if it's empty or not
bool isEmptyMultipleSourceMovesList(multipleSourceMovesList* lst)
{
	if (lst->head == NULL)
		return true;
	else
		return false;
}





