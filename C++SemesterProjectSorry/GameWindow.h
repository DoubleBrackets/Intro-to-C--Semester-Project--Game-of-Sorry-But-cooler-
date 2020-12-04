
#pragma once
#include <SFML/Graphics.hpp>


#define RES_SCALE 2.5;

class GameWindow
{
public:
	int width, height;
	sf::RenderWindow window;
	sf::Event event;
	GameWindow(int, int);
	void PollEvents();
	void DrawSprite(sf::Sprite);
	void Render();
};

