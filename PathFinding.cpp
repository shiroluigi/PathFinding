#include "PathFinding.h"

PathFinding::PathFinding(const char* name) : name(name)
{
	init();
}

PathFinding::~PathFinding()
{
	delete window;
	for (auto* a : states)
	{
		delete a;
	}
}

void PathFinding::init()
{
	window = new sf::RenderWindow(sf::VideoMode(900, 600), name, sf::Style::Close );
	isRunning = true;
	states.emplace_back(new bfs(window));
}

const bool const PathFinding::isRunningQuery() const
{
	return isRunning;
}

void PathFinding::render()
{
	window->clear(sf::Color::Black);
	
	states[0]->render();

	window->display();
}

void PathFinding::update()
{

}

void PathFinding::polls()
{
	sf::Event e;
	while (window->pollEvent(e))
	{
		switch (e.type)
		{
		case sf::Event::Closed:
		{
			isRunning = false;
		}
		case sf::Event::KeyPressed:
		{
			if (e.key.code == sf::Keyboard::Escape)
				isRunning = false;
		}
		default:
			break;
		}
	}
}

void PathFinding::run()
{
	polls();
	render();
	update();
}

