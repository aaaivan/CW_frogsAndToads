#ifndef DECLARATIONS_H
#define DECLARATIONS_H

struct Board
{
	int length=0;
	char *array=nullptr;
};

int askBoardLength();
int askNumOfTokens(int boardLength);
bool playAgain();
char *createBoard(int boardLength);
void populateBoard(Board &board, int tokens);
void printBoard(const Board &board, bool includeFog);
bool playerHasMovesLeft(const Board &board);
void moveFrog(Board &board);
bool moveToad(Board &board);

#endif // !DECLARATIONS_H