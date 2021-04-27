#ifndef TERMINAL
#define TERMINAL

#include <iostream>
#include <Windows.h>
#include <wchar.h>
#include <vector>


template<typename T>
class Matrix;

namespace dims
{
	const SHORT WIDTH = 20;
	const SHORT HEIGHT = 20;
	const SHORT SIZE = WIDTH * HEIGHT;
}

class Terminal
{
private:
	HANDLE hConsole;
	//Matrix<wchar_t> *display;
	CHAR_INFO* buffer;
	SHORT width;
	SHORT height;
	SHORT pixelSize;
	SMALL_RECT consoleArea;
	wchar_t display_char = 0x2584;

	bool setFont(SHORT width, SHORT height);
	bool setSize(SMALL_RECT& area);
	bool removeCursor();

public:
	Terminal(CHAR_INFO ptr[], SHORT pixelSize/*int width0, int height0*/);
	bool activate();
	void draw();
	void drawPixel(int x, int y, unsigned short color);
	void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
	void drawRect(int x, int y, int width, int height);
	void drawBezier(int x, int y, int z);
	void clear();
	void changePixelChar(wchar_t new_char);
	//void setColorMode();
	//void loadPicture();
};


#endif TERMINAL