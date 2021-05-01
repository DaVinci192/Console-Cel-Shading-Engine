#pragma once
#ifndef STACKMAZEGENERATOR
#define STACKMAZEGENERATOR

#include <stack>


#include "terminal.h"

// a maze generator inspired by the algorithm from Javidx9's video
class MazeGenerator : public virtual Terminal
{
private:
	int cellWidth;
	int wallWidth;
	int gridHeight;
	int gridWidth;
	int gridArea;
	SHORT backgroundColor;


public:
	MazeGenerator(SHORT width0, SHORT height0, SHORT pixelSize0, int cellWidth0, int wallWidth0, SHORT backgroundColor0 = 0) : Terminal(width0, height0, pixelSize0)
	{
		cellWidth = cellWidth0;
		wallWidth = wallWidth0;

		if (height0 % (cellWidth + wallWidth) - wallWidth != 0 ||
			width0 % (cellWidth + wallWidth) - wallWidth != 0)
			throw std::invalid_argument("cellWidth and/or wallWidth mismatched with terminal dimensions!");

		gridHeight = height0 / (cellWidth + wallWidth);
		gridWidth = width0 / (cellWidth + wallWidth);
		gridArea = gridHeight * gridWidth;
		backgroundColor = backgroundColor0;
	}

	struct Tile
	{
		int x;
		int y;
		bool visited;
		Tile(int i, int j) : x(i), y(j), visited(false) {}
		Tile() : x(0), y(0), visited(false) {}
	};

	std::vector<Tile> setupMaze(std::stack<Tile*>& log, int startCellX = 0, int startCellY = 0)
	{
		std::vector<Tile> tiles;

		cellWidth = 3;
		wallWidth = 1;

		for (int j = 0; j < (*this).getHeight(); j++)
		{
			for (int i = 0; i < (*this).getWidth(); i++)
			{
				// draw grid
				if (i % (cellWidth + wallWidth) == 0 ||
					j % (cellWidth + wallWidth) == 0)
				{
					(*this).drawPixel(i, j, FOREGROUND_INTENSITY);
				}

				// get center or grid squares, assign
				if (i % (cellWidth + wallWidth) == (cellWidth + wallWidth) / 2 &&
					j % (cellWidth + wallWidth) == (cellWidth + wallWidth) / 2)
				{
					//(*this).drawPixel(i, j, FOREGROUND_BLUE);
					tiles.push_back(Tile(i, j));
				}
			}
		}
		(*this).draw();
		Tile* startCell = &tiles[startCellX + gridWidth * startCellY];
		startCell->visited = true;
		//std::cout << std::endl << startCell->x << std::endl;
		log.push(startCell);

		return tiles;
	}

	void createPath(const Tile& currentLoc, const Tile& nextLoc)
	{
		int wallCenterX = currentLoc.x + (nextLoc.x - currentLoc.x) / 2;
		int wallCenterY = currentLoc.y - (currentLoc.y - nextLoc.y) / 2;

		//std::cout << std::endl << currentLoc.x << std::endl;

		(*this).drawPixel(wallCenterX, wallCenterY, backgroundColor);
		//std::cout << std::endl << "hmmmmmmmmmmmmmmm" << std::endl;

		if (nextLoc.y == currentLoc.y) // vertical wall
		{
			for (int i = 1; i <= cellWidth / 2; i++)
			{
				(*this).drawPixel(wallCenterX, wallCenterY + i, backgroundColor);
				(*this).drawPixel(wallCenterX, wallCenterY - i, backgroundColor);
			}
		}
		else if (nextLoc.x == currentLoc.x) // horizontal wall
		{
			for (int i = 1; i <= cellWidth / 2; i++)
			{
				(*this).drawPixel(wallCenterX + i, wallCenterY, backgroundColor);
				(*this).drawPixel(wallCenterX - i, wallCenterY, backgroundColor);
			}
		}
		//(*this).draw();
	}

	bool findPath(std::vector<Tile>& tiles, std::stack<Tile*>& log)
	{
		if (log.size() == 0)
			return false;

		std::vector<Tile*> paths;

		int gridX = (log.top()->x - wallWidth) / (wallWidth + cellWidth);
		int gridY = (log.top()->y - wallWidth) / (wallWidth + cellWidth);

		//std::cout << std::endl << gridX << std::endl;

		if (tiles[gridX + gridY * gridWidth].y - cellWidth > 0 && // North
			!tiles[gridX + (gridY - 1) * gridWidth].visited)
		{
			paths.push_back(&tiles[gridX + (gridY - 1) * gridWidth]);
		}
		if (tiles[gridX + gridY * gridWidth].x + cellWidth < (*this).getWidth() && // East
			!tiles[(gridX + 1) + gridY * gridWidth].visited)
		{
			paths.push_back(&tiles[(gridX + 1) + gridY * gridWidth]);
		}
		if (tiles[gridX + gridY * gridWidth].y + cellWidth < (*this).getHeight() && // South
			!tiles[gridX + (gridY + 1) * gridWidth].visited)
		{
			paths.push_back(&tiles[gridX + (gridY + 1) * gridWidth]);
		}
		if (tiles[gridX + gridY * gridWidth].x - cellWidth > 0 && // West
			!tiles[(gridX - 1) + gridY * gridWidth].visited)
		{
			paths.push_back(&tiles[(gridX - 1) + gridY * gridWidth]);
		}

		if (paths.size() == 0)
			return false;

		Tile* nextLoc = paths[rand() % paths.size()];
		//std::cout << std::endl << " " << gridX << " " << gridY << std::endl;
		createPath(tiles[gridX + gridWidth * gridY], *nextLoc);
		(*nextLoc).visited = true;
		log.push(nextLoc);

		return true;
	}

	void backTrack(std::stack<Tile*>& log)
	{
		if (log.size() == 0)
			return;

		log.pop();
	}

	void update(float fElapsedTime) override
	{

	}

	void run() override
	{
		if (!(*this).activate())
			(*this).clear();

		auto tp1 = std::chrono::system_clock::now();
		auto tp2 = std::chrono::system_clock::now();

		std::stack<Tile*> log;

		std::vector<Tile> cells = setupMaze(log);

		while (1)
		{
			tp2 = std::chrono::system_clock::now();
			std::chrono::duration<float> elapsedTime = tp2 - tp1;
			tp1 = tp2;
			float fElapsedTime = elapsedTime.count();

			if (!findPath(cells, log))
				backTrack(log);

			Sleep(10);


			(*this).draw();
		}
	}

};

#endif STACKMAZEGENERATOR