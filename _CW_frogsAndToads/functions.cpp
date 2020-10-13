#include "declarations.h"
#include <vector>
#include <cstdlib> // for std::rand()
#include <limits> //for numeric_limits
#include <string>
#include <cctype> // for toupper()
#include <iostream>

const int BOARD_SIZE_MIN{ 3 };
const int BOARD_SIZE_MAX{ 30 };

//return board length entered by the player
int askBoardLength()
{
	int length{};
	while (true)
	{
		std::cout << "How many tiles long should the board be? Enter an integer between "<<BOARD_SIZE_MIN<<" and "<<BOARD_SIZE_MAX<<": ";
		std::cin >> length;
		//input validation
		if (std::cin.fail())	//if the extraction fails, clear the buffer and print error message
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Invalid input. Try again." << std::endl;
		}
		else if (length < BOARD_SIZE_MIN || length > BOARD_SIZE_MAX)	//if length is out of range, print error message
		{
			std::cout << "Your input is out of range. Try again." << std::endl;
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //remove any residual input in the buffer
		}
		else
		{
			std::cout << std::endl;
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //remove any residual input in the buffer
			return length;
		}
	}
}

//return number of tokens entered by the player
int askNumOfTokens(int boardLength)
{
	int tokens{};
	while (true)
	{
		std::cout << "How many tokens do you want to play with? ";
		std::cin >> tokens;
		//input validation
		if (std::cin.fail())	//if the extraction fails, clear the buffer and print error message
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Invalid input. Try again." << std::endl;
		}
		else if (tokens > (boardLength-1)/2)	//if tokens do not fit on the board, print error message
		{
			std::cout << "Too many tokens. Try again." << std::endl;
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //remove any residual input in the buffer
		}
		else if (tokens < 1)	//at least one token required
		{
			std::cout << "Too few tokens. Try again." << std::endl;
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //remove any residual input in the buffer
		}
		else
		{
			std::cout << std::endl;
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //remove any residual input in the buffer
			return tokens;
		}
	}
}

//ask user if they want to play again
bool playAgain()
{
	while (true)
	{
		std::cout << "Do you want to play again(y/n)? ";
		std::string tempStr{};
		char input{};
		std::cin >> tempStr;
		input = toupper(tempStr[0]);
		//input vaidation
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //remove any residual input in the buffer
		if (tempStr.length() != 1)
			std::cout << "Invalid input. Try again." << std::endl;
		else if (input == 'N')
			return false;
		else if (input == 'Y')
			return true;
		else
			std::cout << "Invalid input. Try again." << std::endl;
	}
}

//create a dynamically allocated array of chars.
char *createBoard(int boardLength)
{
	//dynamically allocate an array of size "boardLength"
	char *ptr = new char[boardLength];
	//fill the array with whitespace characaters
	for (int count = 0; count < boardLength; count++)
		ptr[count] = ' ';
	return ptr;
}

//add frogs ('F') and toads ('T') to the board array
void populateBoard(Board &board, int tokens)
{
	for (int count = 0; count < tokens; count++)
	{
		board.array[count] = 'F'; //add frogs on the left
		board.array[board.length-1-count] = 'T'; //add toads on the right
	}
}

//check whether the player has moves left
bool playerHasMovesLeft(const Board &board)
{
	//if at least one of the two tiles directly on the right of a frog
	//is empty, return true.
	for (int count = 0; count < board.length - 2; count++)
	{
		if (board.array[count] == 'F')
		{
			if (board.array[count + 1] == ' ' || board.array[count + 2] == ' ')
				return true;
		}
	}
	//check the last two tiles, which were left out of the loop
	if (board.array[board.length - 2] == 'F' && board.array[board.length - 1] == ' ')
		return true;
	return false;
}

//return the number of the tile the player wants to move a frog from
int askNextMove(const Board  &board)
{
	int input{};
	while (true)
	{
		std::cout << "Which tile do you want to move from? Enter the tile number: ";
		std::cin >> input;
		//input validation
		if (std::cin.fail())	//if the extraction fails, clear the buffer and print error message
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Invalid input. Try again." << std::endl;
		}
		else if (input < 1 || input > board.length)	//if input is not between 1 and the length of the board, print error message
		{
			std::cout << "Your input is out of range. Try again." << std::endl;
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //remove any residual input in the buffer
		}
		else
		{
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //remove any residual input in the buffer
			return input;
		}
	}
}

//ask next move to the player and perform it if possible.
//keep asking until a valid move is entered.
void moveFrog(Board &board)
{
	while (true)
	{
		int moveFromIndex{ askNextMove(board) - 1 }; //when the board is printed the tiles are numbered starting from one,
													 //while the array indexing starts from zero.
		if (board.array[moveFromIndex] != 'F')
			std::cout << "No frog on the selected tile. Try again." << std::endl;
		else
		{
			//first, check whether the frog can move ONE tile to the right.
			//note: "moveFromIndex" cannot be the last index of the array.
			if (moveFromIndex < board.length - 1 && board.array[moveFromIndex + 1] == ' ')
			{
				board.array[moveFromIndex + 1] = 'F';
				board.array[moveFromIndex] = ' ';
				std::cout << std::endl;
				return;
			}
			//second, check whether the frog can move TWO tiles to the right.
			//note: "moveFromIndex" cannot be any of the last two index of the array.
			else if (moveFromIndex < board.length - 2 && board.array[moveFromIndex + 2] == ' ')
			{
				board.array[moveFromIndex + 2] = 'F';
				board.array[moveFromIndex] = ' ';
				std::cout << std::endl;
				return;
			}
			else
				std::cout << "Your frog cannot move from there. Try again." << std::endl;
		}
	}
}

//check if at least one toad can be moved. If not return false.
//If yes, make a random move among the valid ones and return true.
bool moveToad(Board &board)
{
	std::vector<int> validIndexes; //contains all the array's indexes from which a toad can move from
	for (int count = board.length - 1; count > 1; count--)
	{
		//if at least one of the two tiles directly on the left of a toad
		//is empty, add that toad's array's index to "validIndexes".
		if (board.array[count] == 'T')
		{
			if (board.array[count - 1] == ' ' || board.array[count - 2] == ' ')
				validIndexes.push_back(count);
		}
	}
	//check the first two tiles, which were left out of the loop
	if (board.array[1] == 'T' && board.array[0] == ' ')
		validIndexes.push_back(1);

	if (validIndexes.size() == 0) //no valid index found
		return false;
	int moveFromIndex{ validIndexes.at(std::rand() % validIndexes.size()) }; //get a random element of the vector "validIndexes"
	if (board.array[moveFromIndex - 1] == ' ') //move toad one tile to the left, if possible...
	{
		board.array[moveFromIndex - 1] = 'T';
		board.array[moveFromIndex] = ' ';
	}
	else //...otherwise move the toad two tiles to the right
	{
		board.array[moveFromIndex - 2] = 'T';
		board.array[moveFromIndex] = ' ';
	}
	return true;
}
