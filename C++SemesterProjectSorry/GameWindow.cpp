#include "GameWindow.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>



GameWindow::GameWindow(int width, int height)
{
	//Scales resolution
	width *= RES_SCALE;
	height *= RES_SCALE;
	this->width = width;
	this->height = height;
	//Creates sfml window
	window.create(sf::VideoMode(width, height), "Sorry! ^_^");
	window.setKeyRepeatEnabled(false);
	window.setVerticalSyncEnabled(true);
}

void GameWindow::PollEvents()
{
	while (window.pollEvent(event))
	{
		switch (event.type)
			{
			case sf::Event::Closed:
			{
				window.close();
				break;
			}
			case sf::Event::MouseMoved:
			{
				//std::cout << event.mouseMove.x << " " << event.mouseMove.y << std::endl;
				break;
			}
			case sf::Event::MouseButtonReleased:
			{
				std::cout << "eat lead" << std::endl;
			}
		}
	}
}


//Rendering and visuals
void GameWindow::DrawSprite(sf::Sprite sprite)
{
	window.draw(sprite);
}

void GameWindow::Render()
{
	window.clear();
	/*std::list<ObjectSprite>::iterator it;
	int c = 0;
	for (it = ObjectSprite::spriteList.begin(); it != ObjectSprite::spriteList.end(); it++)
	{
		window.draw(it->sprite);
	}*/
	sf::RectangleShape testShape(sf::Vector2f(20, 20));
	testShape.setPosition(sf::Vector2f(width / 2, height / 2));
	testShape.setFillColor(sf::Color::White);
	window.draw(testShape);
	window.display();
}