
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "GameWindow.h"
#include "GameObject.h"


GameWindow::GameWindow(int _width, int _height)
{
	//Scales resolution
	_width *= RES_SCALE;
	_height *= RES_SCALE;
	width = _width;
	height = _height;
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
				GameObject *g = GameObject::FindObject("game_board");
				sf::Vector2f pos = (*g).position;
				pos.x += 5;
				(*g).SetPosition(pos);
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
	window.clear(sf::Color::White);
	std::vector<GameObject>::iterator it;
	int c = 0;
	for (it = GameObject::spriteList.begin(); it != GameObject::spriteList.end(); it++)
	{
		window.draw(it->currentSprite);
	}
	window.display();
}