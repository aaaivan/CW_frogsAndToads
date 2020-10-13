#include "declarations.h"
#include <cstdlib> // for std::srand()
#include <ctime> // for std::time()
#include <iostream>

int main()
{
	std::srand(static_cast<unsigned int>(std::time(nullptr))); // set seed value for rand().

	std::cout << "--------------------------------- TOADS AND FROGS ----------------------------------\n"
		<< "RULES:\nAt each turn, move one of your frogs. You can move it one tile to the right\n"
		<< "or jump over another frog or toad, as long as the destination tile is empty.\n"
		<< "Similarly, the computer will move its toads to the left. First who cannot move\n"
		<< "further looses. You can only see the content of tiles that are at most two away\n"
		<< "from your frogs. Good luck!\n" << std::endl;

	Board board;
	//MAIN LOOP: exited when player does not want to play again
	while (true)
	{
		board.length = askBoardLength();	//set length of the board
		board.array = createBoard(board.length);	//create empty board
		populateBoard(board, askNumOfTokens(board.length));	//add frogs and toads to the board
		//GAME LOOP: exited when a game ends
		while (true)
		{
			printBoard(board, true);	//print board with fog
			if (!playerHasMovesLeft(board))
			{
				std::cout << "You have no moves left. You loose!\n" << std::endl;
				break;
			}
			else
				moveFrog(board);
			if (!moveToad(board))	//check if the computer has at least one valid move. If yes, execute a random one.
			{
				std::cout << "The computer has no moves left. You win!\n" << std::endl;
				break;
			}
		}
		printBoard(board, false);	//print board without fog
		std::cout << std::endl;
		if (playAgain())
		{
			delete[] board.array;
			board.array = nullptr;
			std::cout << "*********************************************************************************\n" << std::endl;
		}
		else
			break;
	}
}