#include <iostream>

#include <cuda_runtime.h>
#include <cassert>

#include <vector>;
#include <algorithm>;

#include "device_launch_parameters.h"

#include <wchar.h>
#include <Windows.h>

#include "algebra.cuh"
#include "terminal.h"

#include <stdlib.h>
#include <time.h>
#include "wchar.h"

using namespace algebra;

void drawNoise(Terminal screen)
{
	for (int i = 0; i < dims::HEIGHT; i++)
	{
		for (int j = 0; j < dims::WIDTH; j++)
		{
			//screen.drawPixel(i, j, FOREGROUND_BLUE | BACKGROUND_RED, rand() % 7 + 0x2582);
			screen.drawPixel(i, j, rand() % 256);
		}
	}
}

// alpha from 0 to 69 expected
void drawGrayscale(int x, int y, int alpha, Terminal screen)
{
	const int PALETTE_SIZE = 69;
	const wchar_t PALETTE[PALETTE_SIZE] = L"$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\|()1{}[]?-_+~<>i!lI;:,\"^`'.";

	screen.drawPixel(x, y, FOREGROUND_INTENSITY, PALETTE[PALETTE_SIZE - alpha]);
}

void drawStaticGradient(Terminal screen, int& alpha)
{
	for (int x = 0; x < dims::WIDTH; x++)
	{
		alpha += int(100 / dims::WIDTH);
		for (int y = 0; y < dims::HEIGHT; y++)
		{
			drawGrayscale(x, y, alpha, screen);
		}
	}
}



int main()
{
	srand(time(0));

	CHAR_INFO ptr[dims::SIZE];

	Terminal Screen = Terminal(ptr, 5);

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

}
