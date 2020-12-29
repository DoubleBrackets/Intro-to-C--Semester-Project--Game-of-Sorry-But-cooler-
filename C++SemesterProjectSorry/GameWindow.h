
#pragma once
#include <SFML/Graphics.hpp>




class GameWindow
{
public:
	sf::View view;
	static GameWindow* instance;
	static double res_Scale;
	int width, height;
	sf::RenderWindow window;
	sf::Event event;
	sf::View GetLetterboxView(sf::View, int, int);
	GameWindow(int, int);
	void PollEvents();
	void DrawSprite(sf::Sprite);
	void Render();
};

