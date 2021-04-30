#include <iostream>

#include <cuda_runtime.h>
#include <cassert>

#include <vector>;
#include <algorithm>;
#include <stack>

#include "device_launch_parameters.h"

#include <wchar.h>
#include <Windows.h>

#include "algebra.cuh"
#include "terminal.h"

#include <stdlib.h>
#include <time.h>
#include "wchar.h"


using namespace algebra;

void drawNoise(Terminal& screen)
{
	
}

class NoiseEngine : public virtual Terminal
{
public:
	NoiseEngine(SHORT width0, SHORT height0, SHORT pixelSize0) : Terminal(width0, height0, pixelSize0) {}


	void update(float fElapsedTime) override
	{
		for (int i = 0; i < (*this).getWidth(); i++)
		{
			for (int j = 0; j < (*this).getHeight(); j++)
			{
				//screen.drawPixel(i, j, FOREGROUND_BLUE | BACKGROUND_RED, rand() % 7 + 0x2582);
				(*this).drawPixel(i, j, rand() % 256);
			}
		}
		(*this).draw();
	}
};

//const wchar_t PALETTE[69] = L"$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\|()1{}[]?-_+~<>i!lI;:,\"^`'.";


class MazeGenerator : public virtual Terminal
{
public:
	MazeGenerator(SHORT width0, SHORT height0, SHORT pixelSize0) : Terminal(width0, height0, pixelSize0) {}
	
	void update(float fElapsedTime) override
	{
		int cellWidth = 3;
		int wallWidth = 1;

		for (int i = 0; i < (*this).getWidth(); i++)
		{
			for (int j = 0; j < (*this).getHeight(); j++)
			{
				if (i % (cellWidth + wallWidth) == 0 ||
					j % (cellWidth + wallWidth) == 0)
				{
					(*this).drawPixel(i, j, FOREGROUND_INTENSITY);
				}


			}
		}
		(*this).draw();
	}

};


int main()
{
	srand(time(0));

	MazeGenerator Screen = MazeGenerator(61, 61, 15);


	Screen.run();

}
