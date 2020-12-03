
#include <SFML/Graphics.hpp>

#include "GameObject.h"
#include "GameWindow.h"

#include <iostream>
#include "ResourceManager.h"

std::vector<GameObject> GameObject::spriteList;
GameObject::GameObject(std::string name)
{
	objectName = name;

}


void GameObject::SetTexture(std::string textureTag,int scale)
{
	std::map<std::string, sf::Texture>::iterator it;
	it = ResourceManager::textureMap.find(textureTag);

	if (it != ResourceManager::textureMap.end())
	{
		currentSprite.setTexture(it->second);
	}
	int temp = RES_SCALE;
	currentSprite.setScale(sf::Vector2f(scale * temp, scale * temp));
	//currentSprite.setTextureRect(sf::IntRect(0, 100, 50, 50));
}

void GameObject::InitializeToRenderList()
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


void GameObject::AddAnimation(std::string name,int framecount, double interval, sf::Vector2f framesize, int columns, int rows, sf::Vector2f startPos)
{
	Animation anim;
	anim.frameCount = framecount;
	anim.frameInterval = interval;
	anim.name = name;
	//Slice
	int counter = 0;
	for (int y = 0; y < rows && counter < framecount; y++)
	{
		for (int x = 0; x < columns && counter < framecount; x++)
		{
			sf::Vector2f cPos = startPos;
			cPos.y += y * framesize.y;
			cPos.x += x * framesize.x;			
			anim.frames[counter] = sf::IntRect(cPos.x, cPos.y, framesize.x, framesize.y);
			counter++;
		}
	}
	animationList.push_back(anim);
}


void GameObject::SetSpriteToFirstFrameOfAnimation(std::string name)
{
	int l = animationList.size();
	for (int x = 0; x < l; x++)
	{
		if (animationList[x].name == name)
		{
			currentSprite.setTextureRect(animationList[x].frames[0]);
		}
	}
}

void GameObject::StartAnimation(std::string name)
{
	int l = animationList.size();
	for (int x = 0; x < l; x++)
	{
		if (animationList[x].name == name)
		{
			currentAnimationIndex = x;
		}
	}
}

void GameObject::StopAnimation()
{
	currentAnimationIndex = -1;
}



