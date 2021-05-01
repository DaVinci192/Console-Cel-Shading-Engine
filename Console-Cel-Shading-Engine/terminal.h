#pragma once
#ifndef TERMINAL
#define TERMINAL

#include <iostream>
#include <Windows.h>
#include <wchar.h>
#include <vector>
#include <WinCon.h>
#include <WinUser.h>
#include <chrono>

template<typename T>
class Matrix;

class Terminal
{
private:
	HANDLE hConsole;
	std::vector<CHAR_INFO> pixels;
	CHAR_INFO* buffer;
	SHORT width;
	SHORT height;
	SHORT size;
	SHORT pixelSize;
	SMALL_RECT consoleArea;

	bool setFont(SHORT width, SHORT height);
	bool setSize(SMALL_RECT& area);
	bool removeCursor();

public:
	Terminal(SHORT width0, SHORT height0, SHORT pixelSize/*int width0, int height0*/);
	~Terminal();
	virtual void run();
	virtual void update(float fElapsedTime) = 0;
	SHORT getWidth();
	SHORT getHeight();
	bool activate();
	void draw();
	void drawPixel(int x, int y, unsigned short color, wchar_t glyph = 0x2588);
	//void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
	//void drawRect(int x, int y, int width, int height);
	//void drawBezier(int x, int y, int z);
	void clear();
	//void setColorMode();
	//void loadPicture();
};


#endif TERMINAL

// ------------------------------------------------------------------------- //
// -------------------------------Application------------------------------- //
// ------------------------------------------------------------------------- //


Terminal::Terminal(SHORT width0, SHORT height0,/*CHAR_INFO ptr[]*/ SHORT pixelSize0)
{
	width = width0;//dims::WIDTH;
	height = height0;//dims::HEIGHT;
	size = width * height;
	hConsole = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		CONSOLE_TEXTMODE_BUFFER,
		NULL
	);
	consoleArea = { 0, 0, 1, 1 };
	buffer = new CHAR_INFO[width * height];
	pixelSize = pixelSize0;
}

Terminal::~Terminal()
{
	delete[] buffer;
	buffer = NULL;
}

void Terminal::run()
{
	if (!(*this).activate())
		(*this).clear();

	// timing code taken from Javidx9: https://github.com/OneLoneCoder/videos/blob/master/olcConsoleGameEngine.h

	auto tp1 = std::chrono::system_clock::now();
	auto tp2 = std::chrono::system_clock::now();

	

	while (1) // implement some exit parameter in the future
	{
		tp2 = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float fElapsedTime = elapsedTime.count();

		(*this).update(fElapsedTime);
	}


}

SHORT Terminal::getWidth()
{
	return width;
}

SHORT Terminal::getHeight()
{
	return height;
}

bool Terminal::setFont(SHORT width0, SHORT height0)
{
	_CONSOLE_FONT_INFOEX font;
	font.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	font.nFont = 0;
	font.dwFontSize = { width0, height0 };
	font.FontFamily = TMPF_DEVICE;
	font.FontWeight = 400; // normal weight according to windows docs https://docs.microsoft.com/en-us/windows/console/console-font-infoex
	wcscpy_s(font.FaceName, L"pixel");

	return SetCurrentConsoleFontEx(hConsole, FALSE, &font);
}

bool Terminal::setSize(SMALL_RECT& area)
{
	/*
	COORD windowDim;
	CONSOLE_SCREEN_BUFFER_INFO windowInfo;

	GetConsoleScreenBufferInfo(hConsole, &windowInfo);

	windowDim.X = windowInfo.dwSize.X - 2;
	windowDim.Y = windowInfo.dwSize.Y;
	*/
	//ShowScrollBar(GetConsoleWindow(), SB_BOTH, FALSE);

	// https://cecilsunkure.blogspot.com/2011/11/windows-console-game-writing-to-console.html

	//area = { 0, 0, SHORT(width - 1), SHORT(height - 1) };
	area = { 0, 0, SHORT(width - 1), SHORT(height - 1) };

	if (!SetConsoleWindowInfo(hConsole, TRUE, &area) ||
		!SetConsoleScreenBufferSize(hConsole, { width, height }))
		return false;

	return true;
}

bool Terminal::removeCursor()
{
	_CONSOLE_CURSOR_INFO cursor;
	cursor.dwSize = 100;
	cursor.bVisible = FALSE;

	return SetConsoleCursorInfo(hConsole, &cursor);
}

bool Terminal::activate()
{
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	if (hConsole == INVALID_HANDLE_VALUE)
		return false;

	DWORD dwMode = 0;
	if (!GetConsoleMode(hConsole, &dwMode))
		return false;

	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if (!SetConsoleMode(hConsole, dwMode))
		return false;

	//hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	if (!setFont(pixelSize, pixelSize))
		return false;

	if (!setSize(consoleArea))
		return false;

	if (!removeCursor())
		return false;

	return true;

	//SetConsoleActiveScreenBuffer(hConsole);
	//SetConsoleScreenBufferSize(hConsole, { width, height }); 
}

void Terminal::draw()
{
	//buffer[5].Char.UnicodeChar = 'w';

	WriteConsoleOutputW(hConsole, buffer, { width, height }, { 0,0 }, &consoleArea);

	/*
	DWORD dwBytesWritten = 0;

	_CONSOLE_SCREEN_BUFFER_INFO screenInfo;
	GetConsoleScreenBufferInfo(hConsole, &screenInfo);

	bool changePos = false;

	if (screenInfo.dwCursorPosition.X == dims::WIDTH - 1 &&
		screenInfo.dwCursorPosition.Y == dims::HEIGHT - 1)
	{
		changePos = true;
	}

	WriteConsole(hConsole, buffer, dims::SIZE, &dwBytesWritten, NULL);

	if (changePos)
	{
		COORD newPos = { 0, 0 };
		SetConsoleCursorPosition(hConsole, newPos);
	}
	*/

	/*COORD pos = { x, y };
	wchar_t* unicode_array = new wchar_t[width * height];
	unicode_array[x + y * width] = 0x2584;
	wchar_t unicode_text[1] = { 0x2584 };
	LPCWSTR str = unicode_text;
	DWORD len = 1;
	DWORD dwBytesWritten = 0;
	WriteConsoleOutputCharacter(hCon, str, len, pos, &dwBytesWritten);*/
}

void Terminal::drawPixel(int x, int y, unsigned short color, wchar_t glyph)
{
	buffer[x + y * width].Char.UnicodeChar = glyph;
	buffer[x + y * width].Attributes = color;
}

/*
void Terminal::drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3)
{

}

void Terminal::drawRect(int x, int y, int width, int height)
{

}

void Terminal::drawBezier(int x, int y, int z)
{

}
*/
void Terminal::clear()
{
	for (int i = 0; i < size; i++)
	{
		buffer[i].Char.UnicodeChar = 0x0020;
		buffer[i].Attributes = 0x0000;
	}
}
