
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "GameWindow.h"
#include "GameObject.h"


GameWindow::GameWindow(int _width, int _height)
{
	//Scales resolution
	_width *= (double)RES_SCALE;
	_height *= (double)RES_SCALE;
	width = _width;
	height = _height;
	//Creates sfml window
	window.create(sf::VideoMode(width, height), "Sorry! ^_^",sf::Style::Close);
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
				int l = GameObject::spriteList.size();
				sf::Vector2i mPos = sf::Mouse::getPosition(window);
				for (int x = l - 1; x >= 0; x--)
				{
					if ((*GameObject::spriteList[x]).CheckForClick(sf::Vector2f(mPos.x, mPos.y), false))
						break;
				}
				break;
			}
			case sf::Event::MouseButtonPressed:
			{
				int l = GameObject::spriteList.size();
				sf::Vector2i mPos = sf::Mouse::getPosition(window);
				for (int x = l - 1; x >= 0; x--)
				{
					if ((*GameObject::spriteList[x]).CheckForClick(sf::Vector2f(mPos.x, mPos.y), true))
						break;
				}
				break;
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
	std::vector<GameObject*>::iterator it;
	int c = 0;
	for (it = GameObject::spriteList.begin(); it != GameObject::spriteList.end(); it++)
	{
		window.draw((*GameObject::spriteList[it - GameObject::spriteList.begin()]).currentSprite);
	}
	window.display();
}