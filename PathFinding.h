#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "States.h"
#include "bfs.h"

class PathFinding
{
public:
	PathFinding(const char* name);
	~PathFinding();
	void run();
	const bool const isRunningQuery() const;
private:
	void render();
	void update();
	void polls();
	void init();
	sf::RenderWindow* window = nullptr;
	const char* name;
	bool isRunning = true;

	std::vector<States*> states;
};

