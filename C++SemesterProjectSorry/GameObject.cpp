
#include <SFML/Graphics.hpp>

#include "GameObject.h"
#include "GameWindow.h"

#include <iostream>
#include <thread>

#include "ResourceManager.h"
#include "piece.h";
std::vector<std::shared_ptr<GameObject>> GameObject::spriteList;
GameObject::GameObject(std::string name)
{
	objectName = name;
}


void GameObject::SetTexture(std::string textureTag,double scale)
{
	std::map<std::string, sf::Texture>::iterator it;
	//gets texture from resourcemanager map using name, sets current sprite to texture
	it = ResourceManager::textureMap.find(textureTag);

	if (it != ResourceManager::textureMap.end())
	{
		currentSprite.setTexture(it->second);
	}
	double temp = RES_SCALE;
	currentSprite.setScale(sf::Vector2f(scale * temp, scale * temp));
	//currentSprite.setTextureRect(sf::IntRect(0, 100, 50, 50));
}

void GameObject::InitializeObject(std::shared_ptr<GameObject> ptr)
{
	if (isRendering)
		return;
	isRendering = true;
	spriteList.push_back(ptr);
}


void GameObject::DeleteObject()
{
	if (!isRendering)
		return;
	isRendering = false;
	std::vector<std::shared_ptr<GameObject>>::iterator it;
	std::shared_ptr<GameObject> thisptr(this);
	for (it = spriteList.begin(); it != spriteList.end(); it++)
	{
		if (spriteList[it - spriteList.begin()] == thisptr)
		{
			//delete(spriteList[it - spriteList.begin()]);
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

//Finds object in object list using name
std::shared_ptr<GameObject> GameObject::FindObject(std::string name)
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

//Sets current object sprite to certain frame in current animation
void GameObject::SetAnimationFrame(int index)
{
	currentSprite.setTextureRect(animationList[currentAnimationIndex].frames[index]);
}

//Starts playing animation based on name
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

//Stops playing current animation
void GameObject::StopAnimation()
{
	currentAnimationIndex = -1;
}

//Update gameobject per tick
void GameObject::UpdateGameObject(long start_time, long frame_duration)
{
	//Updates animation that is currently playing
	if (currentAnimationIndex != -1)
	{
		//time in milli
		double diff  = ((double)start_time - ConvertToMilli(animationList[currentAnimationIndex].startTime))/1000;
		//finds index of frame
		Animation canim = animationList[currentAnimationIndex];
		int index = (int)(diff / canim.frameInterval) % canim.frameCount;
		SetAnimationFrame(index);
	}
	//Physics updates
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
//Checks if 
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
	//If mouse is released without being over the object, still release
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

void Clickable::OnRelease(bool isHovering)
{
	currentSprite.setColor(sf::Color(255, 255,255, 255));
};

//Player subclass

PlayerPiece::PlayerPiece(int teamnum, std::string name,piece* gamePiece) : Clickable::Clickable(name)
{
	if(teamnum == 0)
		SetTexture("redpiece_tex", 1.2);
	else
		SetTexture("bluepiece_tex", 1.2);
	AddAnimation("walk", 4, 0.2, sf::Vector2f(16, 16), 4, 1, sf::Vector2f(0, 0));
	AddAnimation("idle", 4, 0.35, sf::Vector2f(16, 16), 4, 1, sf::Vector2f(0, 16));
	AddAnimation("die", 4, 0.2, sf::Vector2f(16, 16), 4, 1, sf::Vector2f(0, 32));
	StartAnimation("walk");
	this->gamePiece = gamePiece;
}

void PlayerPiece::UpdatePosition(int boardPos)//updates position of graphic object based on source piece
{
	double scale = RES_SCALE;
	if (boardPos > 7 && boardPos < 13)//on red ladder
	{
		SetPosition(sf::Vector2f(7 * 16 * scale, 159 * scale + scale* 16 * (boardPos-7)));
	}
	else if (boardPos > 27 && boardPos < 33)//on blue ladder
	{
		SetPosition(sf::Vector2f(7 * 16 * scale, 159 * scale + scale * 16 * (boardPos - 27)));
	}
	else if(boardPos != -1)//normal square
	{
		int offset = 0;
		if (boardPos >= 13)
			offset = 5;
		if (boardPos >= 33)
			offset = 10;
		SetPosition(sf::Vector2f((boardPos - offset) * 16 * scale, 159*scale));
	}
}

void PlayerPiece::OnClick()
{
	Clickable::OnClick();
	std::cout << gamePiece->team;
}



