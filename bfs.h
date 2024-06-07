#pragma once
#include "States.h"


enum IDs
{
	TRAVERSABLE = 0,
	NOT_TRAVERSABLE = 1,
	START = 2,
	END = 3,
	TRAVERSED = 4
};

class bfs : public States
{
public:
	bfs(sf::RenderWindow* window);
	~bfs();
	void update();
	void render();
private:
	void init();
	void makeGrid();
	void mouseHandle();
	void renderGrid();
	sf::RectangleShape makeNewRectangle();
	//BFS
	std::vector<std::pair<int, int>> bfs2d(std::vector<std::vector<int>>& grid, std::pair<int, int>start, std::pair<int, int> goal);
	std::vector<std::pair<int, int>> reconstructPath(std::pair<int, int> start, std::pair<int, int> goal, std::vector<std::vector<std::pair<int, int>>>& parent);
	bool isValid(int row, int col, int rows, int cols, std::vector<std::vector<int>>& grid, std::vector<std::vector<bool>>& visited);
	//
	//sf::RenderWindow* bfsinfo;
	sf::RenderWindow* child = nullptr;
	sf::RenderWindow* target = nullptr;
	std::vector<sf::RectangleShape> rectangles;
	sf::RectangleShape rectangle;
	std::vector<sf::RectangleShape*> start;
	std::vector<sf::RectangleShape*> end;
	int padding;
	int m_gridX, m_gridY;
	std::vector<std::vector<int>> grid;
};

