#include <iostream>

#include <cuda_runtime.h>
#include <cassert>

#include <vector>;
#include <algorithm>;

#include "device_launch_parameters.h"

#include <wchar.h>
#include <Windows.h>

#include "algebra.cuh"
#include "terminal.cuh"

#include <stdlib.h>
#include <time.h>

using namespace algebra;

// cmake generates a solution file
// cmake findPackage

/*
namespace dims
{
	const SHORT WIDTH = 140;
	const SHORT HEIGHT = 70;
	const SHORT SIZE = WIDTH * HEIGHT;
}
*/

void drawNoise(Terminal screen)
{
	for (int i = 0; i < dims::HEIGHT; i++)
	{
		for (int j = 0; j < dims::WIDTH; j++)
		{
			screen.drawPixel(i, j, rand() % 256);
		}
	}
}

int main()
{
	srand(time(0));

	CHAR_INFO ptr[dims::SIZE];

	
	Terminal Screen = Terminal(ptr, 15);
	

	if (!Screen.activate())
	{
		std::cout << "there was an error" << std::endl;
	}
	Screen.clear();
	
	
	
	while (1)
	{
		drawNoise(Screen);
		Screen.draw();
		Screen.clear();
	}

	//wprintf(L"\x1b[31mThis text has a red foreground using SGR.31.\r\n");
	//wprintf(L"\x1b[38;2;255;0;0;48;2;0;255;0m▄");
	//printf("\u2580");

	//std::cout << "\u2580" << std::endl;

}
