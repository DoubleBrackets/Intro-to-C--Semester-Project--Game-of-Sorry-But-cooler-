
#include <SFML/Graphics.hpp>

#include "GameObject.h"
#include "GameWindow.h"

#include <iostream>
#include <thread>

#include "ResourceManager.h"

std::vector<GameObject*> GameObject::spriteList;
GameObject::GameObject(std::string name)
{
	objectName = name;
}


void GameObject::SetTexture(std::string textureTag,double scale)
{
	std::map<std::string, sf::Texture>::iterator it;
	it = ResourceManager::textureMap.find(textureTag);

	if (it != ResourceManager::textureMap.end())
	{
		currentSprite.setTexture(it->second);
	}
	double temp = RES_SCALE;
	currentSprite.setScale(sf::Vector2f(scale * temp, scale * temp));
	//currentSprite.setTextureRect(sf::IntRect(0, 100, 50, 50));
}

void GameObject::InitializeObject()
{
	if (isRendering)
		return;
	isRendering = true;
	spriteList.push_back(this);
}


void GameObject::DeleteObject()
{
	if (!isRendering)
		return;
	isRendering = false;
	std::vector<GameObject*>::iterator it;
	for (it = spriteList.begin(); it != spriteList.end(); it++)
	{
		if (spriteList[it - spriteList.begin()] == this)
		{
			delete(spriteList[it - spriteList.begin()]);
			spriteList.erase(it);
			return;
		}
	}
}

void GameObject::MoveObject(sf::Vector2f vec)
{
	SetPosition(position + vec);
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
		if ((*spriteList[x]).objectName == name)
		{
			return (spriteList[x]);
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


void GameObject::SetAnimationFrame(int index)
{
	currentSprite.setTextureRect(animationList[currentAnimationIndex].frames[index]);
}

void GameObject::StartAnimation(std::string name)
{
	int l = animationList.size();
	for (int x = 0; x < l; x++)
	{
		if (animationList[x].name == name)
		{
			currentAnimationIndex = x;
			animationList[x].startTime = std::chrono::high_resolution_clock::now();
		}
	}
}

void GameObject::StopAnimation()
{
	currentAnimationIndex = -1;
}

void GameObject::UpdateGameObject(long start_time, long frame_duration)
{
	if (currentAnimationIndex != -1)
	{
		//time in milli
		double diff  = ((double)start_time - ConvertToMilli(animationList[currentAnimationIndex].startTime))/1000;
		//finds index of frame
		Animation canim = animationList[currentAnimationIndex];
		int index = (int)(diff / canim.frameInterval) % canim.frameCount;
		SetAnimationFrame(index);
	}
	//Velocity update
	MoveObject(sf::Vector2f(velocity.x*(double)frame_duration / 1000.0, velocity.y*(double)frame_duration / 1000.0));

}

long GameObject::ConvertToMilli(std::chrono::high_resolution_clock::time_point point)
{
	auto duration = std::chrono::time_point_cast<std::chrono::milliseconds>(point).time_since_epoch();
	return duration.count();
}

void GameObject::FlipSprite()
{
	sf::Vector2f v = currentSprite.getScale();
	v.x *= -1;
	currentSprite.setScale(v);
}

bool GameObject::CheckForClick(sf::Vector2f mPos,bool push_down)
{
	if (!clickable)
		return false;
	//checks bounds
	if (mPos.x >= position.x && mPos.y >= position.y)
	{
		double yscale = currentSprite.getScale().y;
		double xscale = currentSprite.getScale().x;
		if (mPos.x <= position.x + currentSprite.getTextureRect().width * xscale && mPos.y <= position.y + currentSprite.getTextureRect().height * yscale)
		{
			if (push_down)
			{
				OnClick();
			}
			else
			{
				OnRelease(true);
			}
			return true;
		}
	}
	if (!push_down)
		OnRelease(false);
	return false;
}
//Clickable subclass

Clickable::Clickable(std::string name) : GameObject::GameObject(name) 
{
	clickable = true;
};

void Clickable::OnClick()
{
	currentSprite.setColor(sf::Color(150, 150, 150, 255));
};

void Clickable::OnRelease(bool hover)
{
	currentSprite.setColor(sf::Color(255, 255,255, 255));
};

//Player subclass

PlayerPiece::PlayerPiece(int playernum, std::string name) : Clickable::Clickable(name)
{
	if(playernum == 0)
		SetTexture("redpiece_tex", 1.25);
	else
		SetTexture("bluepiece_tex", 1.25);
	AddAnimation("walk", 4, 0.2, sf::Vector2f(16, 16), 4, 1, sf::Vector2f(0, 0));
	AddAnimation("idle", 4, 0.35, sf::Vector2f(16, 16), 4, 1, sf::Vector2f(0, 16));
	StartAnimation("idle");
}



