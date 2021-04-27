#include <iostream>
using namespace std;

#include <wchar.h>
#include <Windows.h>

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

void drawPixel(int x, int y)
{
	COORD pos = { x, y };
	wchar_t* unicode_array = new wchar_t[width * height];
	unicode_array[x + y * width] = 0x2584;
	wchar_t unicode_text[1] = { 0x2584 };
	LPCWSTR str = unicode_text;
	DWORD len = 1;
	DWORD dwBytesWritten = 0;
	WriteConsoleOutputCharacter(hConsole, str, len, pos, &dwBytesWritten);
}

/*
int main()
{
	HANDLE hConsole;
	setupTerminal(140, 70, hConsole);


	int x = 5; int y = 6;
	
	drawPixel(x, y);

	return 0;
}
*/
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
