#include "declarations.h"
#include <windows.h> //for SetConsoleTextAttribute()
#include <cstdio> // for printf()
#include <iostream>
//it was necessary to move these functions here because of some
//name collisions between headers

//colors are bit-encoded with the first 4 bits referring to the background colour
//and the last 4 bits referring to the text colour (in the format intensity-red-green-blue).
const unsigned short lightGreyOnBlack = 0x7; //default
const unsigned short darkGreyOnLightGrey = 0x78;
const unsigned short yellowOnDarkRed = 0x4e;
const unsigned short greenOnDarkBlue = 0x1a;

//change text and background colours depending on whether
//a tile is foggy or not
void setFogColors(bool foggy)
{
	if (foggy)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), darkGreyOnLightGrey);
	else
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), lightGreyOnBlack);
}

//change text and background colours
//based on the content of a tile.
void setTokenColors(char c)
{
	if (c=='F')
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), greenOnDarkBlue);
	else if(c == 'T')
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), yellowOnDarkRed);
	else
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), lightGreyOnBlack);
}

//check whether a tile should be foggy (return true) or not (return false)
bool tileHasFog(const Board &board, int currentIndex)
{
	//if an 'F' is found anywhere between 2 indexes before and 2 indexes after
	//the current index, then the tile at the current index is NOT foggy
	for (int count = (currentIndex - 2 <= 0 ? 0 : currentIndex - 2); 
		count <= (currentIndex + 2 >= board.length-1 ? board.length-1 : currentIndex + 2); count++)
	{
		if (board.array[count] == 'F')
			return false;
	}
	return true;
}

//If parameter "includeFog" is true
//check for each tile of the board whether it should be foggy or not
//and store the result in an array of booleans (true=fog, false=no fog).
//If parameter "includeFog" is false
//fill the array with all false values.
bool *getFogPattern(const Board &board, bool includeFog)
{
	bool *ptr{ new bool[board.length] };
	if (includeFog)
	{
		for (int count = 0; count < board.length; count++)
		{
			ptr[count] = tileHasFog(board, count); //check if current tile is foggy
		}
	}
	else
	{
		for (int count = 0; count < board.length; count++)
		{
			ptr[count] = false;
		}
	}
	return ptr;
}

void printBoard(const Board &board, bool includeFog)
{
	bool *tilesWithFog{ getFogPattern(board, includeFog) }; //array of booleans of the same length as the board,
															//where the n-th element is true/false if the tile
															//at index n is foggy/not foggy, for each index n in the board.
	//print numbers on top
	for (int count = 0; count < board.length; count++)
	{
		std::cout << "  ";
		printf("%2d", count + 1);
		std::cout << " ";
	}
	std::cout << std::endl;
	//print top edge ot the tiles
	std::cout << ">";
	for (int count = 0; count < board.length; count++)
	{
		setFogColors(tilesWithFog[count]); //change colors based on fog
		std::cout << static_cast<char>(218) << static_cast<char>(196) << static_cast<char>(196)
			<< static_cast<char>(196) << static_cast<char>(191);
	}
	setFogColors(false);
	std::cout <<"<\n>";
	//print tiles'content
	for (int count = 0; count < board.length; count++)
	{
		if (tilesWithFog[count])
		{
			setFogColors(true);
			std::cout << static_cast<char>(179) << " ? "<< static_cast<char>(179);
		}
		else //tile is not foggy
		{
			setFogColors(false);
			std::cout << static_cast<char>(179);
			setTokenColors(board.array[count]); //change color based on tile's content
			std::cout<<" "<<board.array[count]<<" ";
			setFogColors(false);
			std::cout << static_cast<char>(179);
		}
	}
	setFogColors(false);
	std::cout << "<\n>";
	//print bottom edge ot the tiles
	for (int count = 0; count < board.length; count++)
	{
		setFogColors(tilesWithFog[count]); //change colors based on fog
		std::cout << static_cast<char>(192) << static_cast<char>(196) << static_cast<char>(196)
			<< static_cast<char>(196) << static_cast<char>(217);
	}
	setFogColors(false);
	std::cout << "<" << std::endl;

	delete[] tilesWithFog;
}