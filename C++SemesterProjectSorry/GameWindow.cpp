
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "GameWindow.h"
#include "GameObject.h"
#include "GameManager.h"


GameWindow* GameWindow::instance;
double GameWindow::res_Scale;
GameWindow::GameWindow(int _width, int _height)
{
	instance = this;
	//Scales resolution
	_width *= res_Scale;
	_height *= res_Scale;
	width = _width;
	height = _height;
	//Creates sfml window
	window.create(sf::VideoMode(width, height), "Sorry! ^_^",sf::Style::Default);
	window.setKeyRepeatEnabled(false);
	window.setVerticalSyncEnabled(true);

	view.setSize(width,height);
	view.setCenter(view.getSize().x / 2, view.getSize().y / 2);
	window.setView(view);
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
				sf::Vector2f mPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
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
				sf::Vector2f mPos = window.mapPixelToCoords( sf::Mouse::getPosition(window));
				for (int x = l - 1; x >= 0; x--)
				{
					if ((*GameObject::spriteList[x]).CheckForClick(sf::Vector2f(mPos.x, mPos.y), true))
						break;
				}
				break;
			}
			case sf::Event::Resized:
			{
				view = GetLetterboxView(view, event.size.width, event.size.height);
				window.setView(view);
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
	window.clear(sf::Color::Black);
	std::vector<std::shared_ptr<GameObject>>::iterator it;
	int c = 0;
	for (it = GameObject::spriteList.begin(); it != GameObject::spriteList.end(); it++)
	{
		std::shared_ptr<GameObject> g = GameObject::spriteList[it - GameObject::spriteList.begin()];
		if(g->isRendering)
			window.draw(g->currentSprite);
		if (g->hasText)
			window.draw(g->mainText);
	}
	window.display();
}

sf::View GameWindow::GetLetterboxView(sf::View view, int windowWidth, int windowHeight) {

	// Compares the aspect ratio of the window to the aspect ratio of the view,
	// and sets the view's viewport accordingly in order to archieve a letterbox effect.
	// A new view (with a new viewport set) is returned.

	float windowRatio = windowWidth / (float)windowHeight;
	float viewRatio = view.getSize().x / (float)view.getSize().y;
	float sizeX = 1;
	float sizeY = 1;
	float posX = 0;
	float posY = 0;

	bool horizontalSpacing = true;
	if (windowRatio < viewRatio)
		horizontalSpacing = false;

	// If horizontalSpacing is true, the black bars will appear on the left and right side.
	// Otherwise, the black bars will appear on the top and bottom.

	if (horizontalSpacing) {
		sizeX = viewRatio / windowRatio;
		posX = (1 - sizeX) / 2.f;
		//GameWindow::res_Scale = windowHeight / 250;
	}

	else {
		sizeY = windowRatio / viewRatio;
		posY = (1 - sizeY) / 2.f;
		//GameWindow::res_Scale = windowWidth / 480;
	}

	view.setViewport(sf::FloatRect(posX, posY, sizeX, sizeY));

	return view;
}