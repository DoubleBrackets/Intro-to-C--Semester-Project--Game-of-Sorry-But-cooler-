
#include <SFML/Graphics.hpp>

#include "GameObject.h"
#include "GameWindow.h"

#include <iostream>
#include "ResourceManager.h"

std::vector<GameObject> GameObject::spriteList;

GameObject::GameObject(std::string name, std::string s, int x, int y)
{
	objectName = name;
	std::map<std::string, sf::Texture>::iterator it;
	it = ResourceManager::textureMap.find(s);

	if (it != ResourceManager::textureMap.end())
	{
		currentSprite.setTexture(it->second);
	}

	int temp = RES_SCALE;
	currentSprite.setScale(sf::Vector2f(temp, temp));
	SetPosition(sf::Vector2f(x, y));
	//currentSprite.setTextureRect(sf::Rect(sf::Vector2f(0,0),sf::Vector2f(10,10)));
	AddSpriteToRender();

}
GameObject::GameObject(std::string name, std::string s, int x, int y, int scale)
{
	objectName = name;
	std::map<std::string, sf::Texture>::iterator it;
	it = ResourceManager::textureMap.find(s);

	if (it != ResourceManager::textureMap.end())
	{
		currentSprite.setTexture(it->second);
	}

	currentSprite.setScale(sf::Vector2f(scale, scale));
	SetPosition(sf::Vector2f(x, y));
	AddSpriteToRender();

}

void GameObject::AddSpriteToRender()
{
	if (isRendering)
		return;
	isRendering = true;
	spriteList.push_back(*this);
}

void GameObject::RemoveSpriteFromRender()
{
	if (!isRendering)
		return;
	isRendering = false;
	std::vector<GameObject>::iterator it;
	for (it = spriteList.begin(); it != spriteList.end(); it++)
	{
		if (&spriteList[it-spriteList.begin()] == this)
		{
			spriteList.erase(it);
			return;
		}
	}
}

void GameObject::SetPosition(sf::Vector2f pos)
{
	position = pos;
	currentSprite.setPosition(pos);
}


GameObject *GameObject::FindObject(std::string name)
{
	int size = spriteList.size();
	for (int x = 0; x < size; x++)
	{
		if (spriteList[x].objectName == name)
		{
			return &(spriteList[x]);
		}
	}

}



