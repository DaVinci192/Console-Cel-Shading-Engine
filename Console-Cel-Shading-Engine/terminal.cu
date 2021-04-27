﻿#include <iostream>
#include "terminal.cuh"
#include <WinCon.h>
#include <WinUser.h>



Terminal::Terminal(CHAR_INFO ptr[], SHORT pixelSize0)
{
	width = dims::WIDTH;
	height = dims::HEIGHT;
	hConsole = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		CONSOLE_TEXTMODE_BUFFER,
		NULL
	);
	consoleArea = { 0, 0, 1, 1 };
	buffer = ptr;
	pixelSize = pixelSize0;
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

bool Terminal::setSize(SMALL_RECT &area)
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

	/*
	DWORD dwMode = 0;
	if (!GetConsoleMode(hConsole, &dwMode))
		return false;

	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	//dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if (!SetConsoleMode(hConsole, dwMode))
		return false;
	*/
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
	/*COORD pos = { x, y };
	wchar_t* unicode_array = new wchar_t[width * height];
	unicode_array[x + y * width] = 0x2584;
	wchar_t unicode_text[1] = { 0x2584 };
	LPCWSTR str = unicode_text;
	DWORD len = 1;
	DWORD dwBytesWritten = 0;
	WriteConsoleOutputCharacter(hCon, str, len, pos, &dwBytesWritten);*/
}

void Terminal::drawPixel(int x, int y, unsigned short color)
{
	buffer[x + y * width].Char.UnicodeChar = 0x2588;
	buffer[x + y * width].Attributes = color;
}

void Terminal::drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3)
{

}

void Terminal::drawRect(int x, int y, int width, int height)
{

}

void Terminal::drawBezier(int x, int y, int z)
{

}

void Terminal::clear()
{
	for (int i = 0; i < dims::SIZE; i++)
	{
		buffer[i].Char.UnicodeChar = '\u0020';
		buffer[i].Attributes = 0x0002;
	}
}

void Terminal::changePixelChar(wchar_t new_char)
{
	display_char = new_char;
}
 

/*
void setupTerminal(int width, int height, HANDLE& hCon)
{
	hCon = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		CONSOLE_TEXTMODE_BUFFER,
		NULL
	);
	SetConsoleActiveScreenBuffer(hCon);
	SetConsoleScreenBufferSize(hCon, { width, height });
}

void drawPixel(int x, int y, int width, int height, HANDLE hCon)
{
	COORD pos = { x, y };
	wchar_t* unicode_array = new wchar_t[width * height];
	unicode_array[x + y * width] = 0x2584;
	wchar_t unicode_text[1] = { 0x2584 };
	LPCWSTR str = unicode_text;
	DWORD len = 1;
	DWORD dwBytesWritten = 0;
	WriteConsoleOutputCharacter(hCon, str, len, pos, &dwBytesWritten);
}
*/