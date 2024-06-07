#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class States
{
public:
	virtual ~States();
	virtual void update();
	virtual void render();
	
};

