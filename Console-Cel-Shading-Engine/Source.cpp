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
#include <stdio.h>
#include <time.h>
#include "wchar.h"

#include "stackMazeGenerator.h"

using namespace algebra;



// ASCII palette for future art
//const wchar_t PALETTE[69] = L"$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\|()1{}[]?-_+~<>i!lI;:,\"^`'.";

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





int main()
{
	srand(time(0));

	MazeGenerator Screen = MazeGenerator(57, 57, 15, 3, 1);


	Screen.run();

}
