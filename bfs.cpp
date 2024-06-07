#include "bfs.h"
#include <queue>

bfs::bfs(sf::RenderWindow* window) : target(window)
{
	std::cout << "BFS overloaded constructor " << std::endl;
	padding = 0;
	init();
}

bfs::~bfs() 
{
	delete this->child;
	std::cout << "BFS destructor " << std::endl;
}

void bfs::update()
{
}

void bfs::render()
{
	this->child->display();
	renderGrid();
	mouseHandle();
}

void bfs::init()
{
	this->child = new sf::RenderWindow(sf::VideoMode(500, 500),"Info",sf::Style::Titlebar);
	rectangle.setSize(sf::Vector2f(50, 50));
	rectangle.setFillColor(sf::Color::Blue);
	makeGrid();
}

void bfs::makeGrid()
{
	m_gridX = (target->getSize().x - padding) / rectangle.getSize().x ;
	m_gridY = (target->getSize().y - padding) / rectangle.getSize().y ;
	std::cout << m_gridX << " " << m_gridY << std::endl;
	grid.resize(m_gridY);
	for (int i = 0; i < m_gridY ; i++)
		grid[i].resize(m_gridX);
	for (int i = 0; i < m_gridY; i++)
	{
		for (int j = 0; j < m_gridX; j++)
		{
				grid[i][j] = IDs(TRAVERSABLE);
		}
	}
	for (int i = 0; i < m_gridY; i++)
	{
		for (int j = 0; j < m_gridX; j++)
		{
			std::cout << grid[i][j];
		}
		std::cout << "\n";
	}
}


void bfs::renderGrid()
{
	rectangles.clear();
	for (int i = 0; i < m_gridY; i++)
	{
		for (int j = 0; j < m_gridX; j++)
		{
			if (grid[i][j] == IDs(TRAVERSABLE))
			{
				sf::RectangleShape r = makeNewRectangle();
				r.setPosition(sf::Vector2f(j * rectangle.getSize().x, i * rectangle.getSize().x));
				r.setFillColor(sf::Color::Blue);
				rectangles.push_back(r);
			}
			else if (grid[i][j] == IDs(NOT_TRAVERSABLE))
			{
				sf::RectangleShape r = makeNewRectangle();
				r.setPosition(sf::Vector2f(j * rectangle.getSize().x, i * rectangle.getSize().x));
				r.setFillColor(sf::Color::Black);
				rectangles.push_back(r);
			}
			else if (grid[i][j] == IDs(START))
			{
				sf::RectangleShape r = makeNewRectangle();
				r.setPosition(sf::Vector2f(j * rectangle.getSize().x, i * rectangle.getSize().x));
				r.setFillColor(sf::Color::Green);
				rectangles.push_back(r);
			}
			else if (grid[i][j] == IDs(END))
			{
				sf::RectangleShape r = makeNewRectangle();
				r.setPosition(sf::Vector2f(j * rectangle.getSize().x, i * rectangle.getSize().x));
				r.setFillColor(sf::Color::Red);
				rectangles.push_back(r);
			}
			else if (grid[i][j] == IDs(TRAVERSED))
			{
				sf::RectangleShape r = makeNewRectangle();
				r.setPosition(sf::Vector2f(j * rectangle.getSize().x, i * rectangle.getSize().x));
				r.setFillColor(sf::Color::Magenta);
				rectangles.push_back(r);
			}
		}
	}
	for (auto a : rectangles)
	{
		target->draw(a);
	}
}

sf::RectangleShape bfs::makeNewRectangle()
{
	sf::RectangleShape r;
	r.setSize(rectangle.getSize());
	r.setOutlineColor(sf::Color::Black);
	r.setOutlineThickness(5);
	return r;
}



std::vector<std::pair<int, int>> bfs::bfs2d(std::vector<std::vector<int>>& grid, std::pair<int, int>start, std::pair<int, int> goal)
{
	int rows = grid.size();
	int cols = grid[0].size();
	//Directions for movement: vector of pairs
	std::vector<std::pair<int, int>> directions = { {0,1},{1,0},{0,-1},{-1,0} };
	//Queue for bfs and visited array
	std::queue<std::pair<int, int>> q;
	std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
	//Parent array for path reconstruction
	std::vector<std::vector<std::pair<int, int>>> parent(rows, std::vector<std::pair<int, int>>(cols, { -1,-1 }));
	//Initialize bfs 
	q.push(start);
	visited[start.first][start.second] = true;
	parent[start.first][start.second] = start;
	//bfs loop
	while (!q.empty())
	{
		std::pair<int, int> current = q.front();
		q.pop();
		//check if goal is reached
		if (current == goal)
		{
			return reconstructPath(start, goal, parent);
		}
		//Explore neighbours
		for (auto& direction : directions)
		{
			int newRow = current.first + direction.first;
			int newCol = current.second + direction.second;

			if (isValid(newRow, newCol, rows, cols, grid, visited))
			{
				std::pair<int, int> neighbor = { newRow, newCol };
				q.push(neighbor);
				visited[newRow][newCol] = true;
				parent[newRow][newCol] = current;
			}
		}
	}
	//if goal not reached 
	return {};
}

std::vector<std::pair<int, int>> bfs::reconstructPath(std::pair<int, int> start, std::pair<int, int> goal, std::vector<std::vector<std::pair<int, int>>>& parent)
{
	std::vector<std::pair<int, int>> path;
	for (std::pair<int, int> at = goal; at != start; at = parent[at.first][at.second]) {
		path.push_back(at);
	}
	path.push_back(start);
	reverse(path.begin(), path.end());
	return path;
}

bool bfs::isValid(int row, int col, int rows, int cols, std::vector<std::vector<int>>& grid, std::vector<std::vector<bool>>& visited)
{
	// Return true if the cell is within the grid bounds, is not an obstacle, and is not visited
	return (row >= 0) && (row < rows) && (col >= 0) && (col < cols) && (grid[row][col] != IDs(NOT_TRAVERSABLE)) && (!visited[row][col]);
}

void bfs::mouseHandle()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(static_cast<sf::Window&>(*target));
		for (auto& a : rectangles)
		{
			if (a.getGlobalBounds().contains(sf::Vector2f(mousePos.x,mousePos.y)))
			{
				
				if (start.size() != 0)
				{
					int col = start[0]->getPosition().x / rectangle.getSize().x;
					int row = start[0]->getPosition().y / rectangle.getSize().y;
					grid[row][col] = IDs(TRAVERSABLE);
					start.clear();
				}
				//Calculate I and J of 'a'
				int col = floor(a.getPosition().x / rectangle.getSize().x);
				int row = floor(a.getPosition().y / rectangle.getSize().y);
				std::cout << row <<" " << col << std::endl;
				grid[row][col] = IDs(START);
				start.emplace_back(&a);
				break;
			}
		}
	}
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(static_cast<sf::Window&>(*target));
		for (auto& a : rectangles)
		{
			if (a.getGlobalBounds().contains(sf::Vector2f(mousePos.x, mousePos.y)))
			{

				if (end.size() != 0)
				{
					int col = end[0]->getPosition().x / rectangle.getSize().x;
					int row = end[0]->getPosition().y / rectangle.getSize().y;
					grid[row][col] = IDs(TRAVERSABLE);
					end.clear();
				}
				//Calculate I and J of 'a'
				int col = floor(a.getPosition().x / rectangle.getSize().x);
				int row = floor(a.getPosition().y / rectangle.getSize().y);
				std::cout << row << " " << col << std::endl;
				grid[row][col] = IDs(END);
				end.emplace_back(&a);
				break;
			}
		}
	}
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(static_cast<sf::Window&>(*target));
		for (auto& a : rectangles)
		{
			if (a.getGlobalBounds().contains(sf::Vector2f(mousePos.x, mousePos.y)))
			{
				//Calculate I and J of 'a'
				if (start.size() != 0) if (&a == start[0]) break;
				if (end.size() != 0) if (&a == end[0]) break;
				int col = a.getPosition().x / rectangle.getSize().x;
				int row = a.getPosition().y / rectangle.getSize().y;
				std::cout << row << " " << col << std::endl;
				grid[row][col] = IDs(NOT_TRAVERSABLE);
				break;
			}
		}
		for (int i = 0; i < m_gridY; i++)
		{
			for (int j = 0; j < m_gridX; j++)
			{
				std::cout << grid[i][j];
			}
			std::cout << "\n";
		}
	}
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(static_cast<sf::Window&>(*target));
		for (auto& a : rectangles)
		{
			if (a.getGlobalBounds().contains(sf::Vector2f(mousePos.x, mousePos.y)))
			{
				if (start.size() != 0) if (&a == start[0]) break;
				if (end.size() != 0) if (&a == end[0]) break;
				int col = a.getPosition().x / rectangle.getSize().x;
				int row = a.getPosition().y / rectangle.getSize().y;
				std::cout << row << " " << col << std::endl;
				grid[row][col] = IDs(TRAVERSABLE);
				break;
			}
		}
		for (int i = 0; i < m_gridY; i++)
		{
			for (int j = 0; j < m_gridX; j++)
			{
				std::cout << grid[i][j];
			}
			std::cout << "\n";
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
	{
		for (auto& a : this->grid)
		{
			for (int i = 0; i < a.size(); i++)
			{
				if (a[i] == IDs(TRAVERSED))
				{
					a[i] = IDs(TRAVERSABLE);
				}
			}
		}
		if (start.size() == 0 || end.size() == 0) 
			return;
		int startCol = start[0]->getPosition().x / rectangle.getSize().x;
		int startRow = start[0]->getPosition().y / rectangle.getSize().y;
		int endCol = end[0]->getPosition().x / rectangle.getSize().x;
		int endRow = end[0]->getPosition().y / rectangle.getSize().y;

		std::cout << "Start" << startRow << " " << startCol << "End" << endRow << " " << endCol << std::endl;
		std::pair<int, int> s = { startRow , startCol };
		std::pair<int, int> g = { endRow,endCol };

		std::vector<std::pair<int, int>> path = bfs2d(this->grid, s, g);

		if (!path.empty()) {
			std::cout << "Path found:\n";
			for (auto& p : path) {
				if (!(p == s || p == g))
					grid[p.first][p.second] = IDs(TRAVERSED);
			}
			std::cout << std::endl;
		}
		else {
			std::cout << "No path found.\n";
		}
		
	}
}