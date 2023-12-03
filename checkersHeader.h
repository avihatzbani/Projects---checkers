

#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BOARD_SIZE 8
#define NO_CAPTURES 0
#define NO_PLAYERS 0
#define LEFT 0
#define RIGHT 1
#define FIRST_ROW 0
#define LAST_ROW 7



//ALL typedefs:

typedef struct checkersPos
{
	char row, col;
}checkersPos;

typedef unsigned char Player;
typedef unsigned char Board[BOARD_SIZE][BOARD_SIZE];

typedef struct SingleSourceMovesTreeNode {
	Board board;
	checkersPos* pos;
	unsigned short total_captures_so_far;
	struct SingleSourceMovesTreeNode* next_move[2];

}SingleSourceMovesTreeNode;

typedef struct SingleSourceMovesListCell {
	checkersPos* position;
	unsigned short captures;
	struct SingleSourceMovesListCell* next;
}SingleSourceMovesListCell;

typedef struct SingleSourceMovesList {
	SingleSourceMovesListCell* head;
	SingleSourceMovesListCell* tail;
}SingleSourceMovesList;

typedef struct _multipleSourceMovesListCell {
	SingleSourceMovesList* single_source_moves_list;
	struct _multipleSourceMovesListCell* next;
}MultipleSourceMovesListCell;

typedef struct multipleSourceMovesList {
	MultipleSourceMovesListCell* head;
	MultipleSourceMovesListCell* tail;
}multipleSourceMovesList;

typedef struct SingleSourceMovesTree {
	SingleSourceMovesTreeNode* source;
}SingleSourceMovesTree;



// functions declaration:


//The tree's functions:
SingleSourceMovesTree* FindSingleSourceMoves(Board board, checkersPos* src);
SingleSourceMovesTreeNode* helperBuildTreeForPlayerT(Board board, int row, int col, int captures);
SingleSourceMovesTreeNode* helperBuildTreeForPlayerB(Board board, int row, int col, int captures);
SingleSourceMovesTreeNode* createNewTreeNode(Board board, unsigned int total_captures_so_far, int row, int col);
int findMaxCaptures(SingleSourceMovesTreeNode* t);
void copyBoardGame(Board resBoard, Board board);
void printTreeInorder(SingleSourceMovesTree* t);
void print_in_order_rec(SingleSourceMovesTreeNode* n);
void freeTree(SingleSourceMovesTree* tr);
void freeTreeHelper(SingleSourceMovesTreeNode* root);




//The list's functions:

SingleSourceMovesList* FindSingleSourceOptimalMove(SingleSourceMovesTree* moves_tree);
void HelperFindSingleSourceOptimalMoveB(SingleSourceMovesTreeNode* moves_tree, int maxCapture, SingleSourceMovesList* lst);
void HelperFindSingleSourceOptimalMoveT(SingleSourceMovesTreeNode* moves_tree, int maxCapture, SingleSourceMovesList* lst);
multipleSourceMovesList* FindAllPossiblePlayerMoves(Board board, Player player);

void makeEmptySingleSourceMovesList(SingleSourceMovesList* lst);
void insertDataToStartSingleSourceMovesList(SingleSourceMovesList* lst, int captures, checkersPos* position);
SingleSourceMovesListCell* createNewSingleSourceMovesListNode(int captures, checkersPos* position, SingleSourceMovesListCell* next);
void insertNodeToStartSingleSourceMovesList(SingleSourceMovesList* lst, SingleSourceMovesListCell* head);
bool isEmptySingleSourceMovesList(SingleSourceMovesList* lst);

void makeEmptyMultipleSourceMovesList(multipleSourceMovesList* lst);
void insertDataToEndMultipleSourceMovesList(multipleSourceMovesList* lst, SingleSourceMovesList* singleList);
MultipleSourceMovesListCell* createNewMultipleSourceMovesListNode(SingleSourceMovesList* singleList, MultipleSourceMovesListCell* next);
void insertNodeToEndMultipleSourceMovesList(multipleSourceMovesList* lst, MultipleSourceMovesListCell* tail);
bool isEmptyMultipleSourceMovesList(multipleSourceMovesList* lst);



//The game's functions:

void startBoard(Board board);
void Turn(Board board, Player player);
void BoardAfterMove(Board board, SingleSourceMovesList* lst);

void playGame(Board board, Player starting_player);
bool playerGetToLastRow(Board board, Player player);
int changeCharToNum(char ch);
int playersCounter(Board board, Player player);

void printBoard(Board board);
void printLine();

void checkMemoryAllocation(void* ptr);
