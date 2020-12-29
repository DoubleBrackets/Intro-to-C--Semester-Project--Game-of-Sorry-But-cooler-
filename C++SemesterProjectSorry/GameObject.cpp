
#include <SFML/Graphics.hpp>

#include "GameObject.h"
#include "GameWindow.h"
#include "GameManager.h"

#include <iostream>
#include <thread>

#include "ResourceManager.h"
#include "piece.h";

void ShowIndicators(std::vector<int> pos);

void HideIndicators();

void UpdateIndicators();

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
	double temp = GameWindow::res_Scale;
	currentSprite.setScale(sf::Vector2f(scale * temp, scale * temp));
	//currentSprite.setTextureRect(sf::IntRect(0, 100, 50, 50));
}

void GameObject::InitializeObject(std::shared_ptr<GameObject> ptr)
{
	if (isInitialized)
		return;
	isInitialized = true;
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
	return NULL;
}


void GameObject::AddAnimation(std::string name,int framecount, double interval, sf::Vector2f framesize, int columns, int rows, sf::Vector2f startPos,bool loop)
{
	Animation anim;
	anim.frameCount = framecount;
	anim.frameInterval = interval;
	anim.name = name;
	anim.looping = loop;
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
	animationPlaying = true;
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
	animationPlaying = false;
}

//Update gameobject per tick
void GameObject::UpdateGameObject(long start_time, long frame_duration)
{
	//Updates animation that is currently playing
	if (currentAnimationIndex != -1 && animationPlaying)
	{
		//time in milli
		double diff  = ((double)start_time - ConvertToMilli(animationList[currentAnimationIndex].startTime))/1000;
		//finds index of frame
		Animation canim = animationList[currentAnimationIndex];
		int index = (int)(diff / canim.frameInterval) % canim.frameCount;
		if (!canim.looping && index == canim.frameCount - 1)//last frame, stops anim
		{
			animationPlaying = false;
		}
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
	if (!clickable || !isRendering)
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
		SetTexture("redpiece_tex", 1);
	else
		SetTexture("bluepiece_tex", 1);
	AddAnimation("walk", 4, 0.2, sf::Vector2f(16, 16), 4, 1, sf::Vector2f(0, 0),true);
	AddAnimation("idle", 4, 0.35, sf::Vector2f(16, 16), 4, 1, sf::Vector2f(0, 16), true);
	AddAnimation("die", 4, 0.2, sf::Vector2f(16, 16), 4, 1, sf::Vector2f(0, 32), true);
	StartAnimation("idle");
	this->gamePiece = gamePiece;
}

void PlayerPiece::UpdatePosition(int boardPos)//updates position of graphic object based on source piece
{
	double scale = GameWindow::res_Scale;
	if (boardPos > 7 && boardPos < 13)//on red ladder
	{
		SetPosition(sf::Vector2f((7 * 16+1) * scale, 160 * scale - scale* 16 * (boardPos-7)));
	}
	else if (boardPos > 27 && boardPos < 33)//on blue ladder
	{
		SetPosition(sf::Vector2f((22 * 16+1) * scale, 160 * scale - scale * 16 * (boardPos - 27)));
	}
	else if(boardPos != -1)//normal square
	{
		int offset = 0;
		if (boardPos >= 13)
			offset = 5;
		if (boardPos >= 33)
			offset = 10;
		SetPosition(sf::Vector2f(((boardPos - offset) * 16+1) * scale, 160*scale));
	}
}

void PlayerPiece::OnRelease(bool isHovering)
{
	Clickable::OnRelease(isHovering);
	if (!isHovering || gamePiece->team != GameManager::instance->currentTurn)
		return;
	GameManager::instance->teamMoveOutOfHome = -1;
	GameManager::instance->selectedPiece = gamePiece;
	UpdateIndicators();
}

void PlayerPiece::ShowCardIndicators()
{
	CardManager::instance->UpdateActiveCards(gamePiece,GameManager::instance->currentCard);
	std::vector<int> pos = GameManager::instance->SelectPiece(gamePiece);
	std::cout << GameManager::instance->PrintPiece(gamePiece) << " has been selected" << std::endl;
	ShowIndicators(pos);
}


//team home subclass
TeamHome::TeamHome(std::string name, int team) : Clickable::Clickable(name)
{
	this->team = team;
	double scale = GameWindow::res_Scale;
	if (team == 0)
	{
		SetTexture("redhome_tex",1);
		SetPosition(sf::Vector2f(9 * 16 * scale, 132 * scale));
	}
	else
	{
		SetTexture("bluehome_tex", 1);
		SetPosition(sf::Vector2f(24 * 16 * scale, 132 * scale));
	}
	AddAnimation("4", 6, 0.25, sf::Vector2f(16, 32), 6, 1, sf::Vector2f(0, 0), true);
	AddAnimation("3", 6, 0.25, sf::Vector2f(16, 32), 6, 1, sf::Vector2f(0, 32), true);
	AddAnimation("2", 6, 0.25, sf::Vector2f(16, 32), 6, 1, sf::Vector2f(0, 64), true);
	AddAnimation("1", 6, 0.25, sf::Vector2f(16, 32), 6, 1, sf::Vector2f(0, 96), true);
	AddAnimation("0", 1, 1, sf::Vector2f(16, 32), 1, 1, sf::Vector2f(0, 128), true);
	StartAnimation("4");
}

void TeamHome::OnRelease(bool isHovering)//home selected
{
	Clickable::OnRelease(isHovering);
	if (!isHovering || team != GameManager::instance->currentTurn)
		return;
	GameManager::instance->selectedPiece = NULL;
	//If clicked, then try to bring new piece onto the field
	std::cout << ((team == 0) ? "red" : "blue") << " team's home has been selected" << std::endl;
	GameManager::instance->teamMoveOutOfHome = team;
	UpdateIndicators();
}

//confirm button
ConfirmButton::ConfirmButton(std::string name) : Clickable::Clickable(name)
{
	double scale = GameWindow::res_Scale;
	SetTexture("confirmbutton_tex",1);
	SetPosition(sf::Vector2f(GameWindow::instance->width - 86 * scale, GameWindow::instance->height -87 * scale));
}

void ConfirmButton::OnRelease(bool isHovering)
{
	Clickable::OnRelease(isHovering);
	if (!isHovering || GameManager::instance->gameEnded)
		return;
	//If clicked, then attempt to move current piece
	if (GameManager::instance->teamMoveOutOfHome != -1)
	{
		if(GameManager::instance->SelectHomePieceToMove(GameManager::instance->teamMoveOutOfHome))
			HideIndicators();
	}
	else if(GameManager::instance->selectedPiece != NULL)
	{
		if (GameManager::instance->SelectPieceForMove(GameManager::instance->selectedPiece))
			HideIndicators();
	}
}

//swap button
SwapButton::SwapButton(std::string name) : Clickable::Clickable(name)
{
	double scale = GameWindow::res_Scale;
	SetTexture("swapbutton_tex", 1);
	SetPosition(sf::Vector2f(GameWindow::instance->width - 86 * scale, GameWindow::instance->height - 45 * scale));
}

void SwapButton::OnRelease(bool isHovering)
{
	Clickable::OnRelease(isHovering);
	if (!isHovering || GameManager::instance->gameEnded)
		return;
	//swaps cards
	GameManager::instance->SwapCard();
	UpdateIndicators();
}

//Indicators

sf::Vector2f BoardposToV2(int boardPos)
{
	int team = GameManager::instance->currentTurn;
	double scale = GameWindow::res_Scale;
	if (boardPos == -2)//reach end
	{
		return(sf::Vector2f(((team==0)?7:22) * 16 * scale, 157 * scale - scale * 16 * 6));
	}
	if (boardPos > 7 && boardPos < 13)//ladders
	{
		return(sf::Vector2f(7 * 16 * scale, 157 * scale - scale * 16 * (boardPos - 7)));
	}
	else if (boardPos > 27 && boardPos < 33)
	{
		return(sf::Vector2f(22 * 16 * scale, 157 * scale - scale * 16 * (boardPos - 27)));
	}
	else//normal square
	{
		int offset = 0;
		if (boardPos >= 13)
			offset = 5;
		if (boardPos >= 33)
			offset = 10;
		return(sf::Vector2f((boardPos - offset) * 16 * scale, 157 * scale));
	}
}

void CreateIndicator(sf::Vector2f pos, int num)
{
	double scale = GameWindow::res_Scale;
	std::shared_ptr<GameObject> indicator = std::make_shared<GameObject>("indicator"+std::to_string(num));
	indicator->SetTexture("selectbox_tex",1);
	indicator->AddAnimation("default", 6, 1.0 / 7, sf::Vector2f(16, 24), 3, 2, sf::Vector2f(0, 0), true);
	indicator->SetPosition(pos);
	indicator->StartAnimation("default");
	indicator->isRendering = false;
	indicator->InitializeObject(indicator);
}

void CreateIndicatorLastPos()
{
	double scale = GameWindow::res_Scale;
	std::shared_ptr<GameObject> indicator = std::make_shared<GameObject>("indicatorlastpos");
	indicator->SetTexture("selectboxlastpos_tex", 1);
	indicator->AddAnimation("default", 6, 1.0 / 7, sf::Vector2f(16, 24), 4, 2, sf::Vector2f(0, 0), true);
	indicator->StartAnimation("default");
	indicator->isRendering = false;
	indicator->InitializeObject(indicator);
}

void ConfirmButton::CreateIndicators()
{
	double scale = GameWindow::res_Scale;
	for (int x = 0; x < 40; x++)
	{
		CreateIndicator(BoardposToV2(x),x);
	}
	CreateIndicatorLastPos();
}


void ShowIndicators(std::vector<int> pos)
{
	int l = pos.size();
	for (int x = 0; x < l-1; x++)
	{
		std::shared_ptr<GameObject> g = GameObject::FindObject("indicator" + std::to_string(pos[x]));
		if (g != NULL)
		{
			g->isRendering = true;
		}
	}
	//last indicator position
	if (l < 1)
		return;
	std::shared_ptr<GameObject> g = GameObject::FindObject("indicatorlastpos");
	g->isRendering = true;
	g->SetPosition(BoardposToV2(pos[l-1]));
}

void UpdateIndicators()
{
	HideIndicators();
	if (GameManager::instance->selectedPiece != NULL)
	{
		GameManager::instance->selectedPiece->pieceObject->ShowCardIndicators();
	}
	else if (GameManager::instance->teamMoveOutOfHome != -1)
	{
		std::vector<int> indPos;
		indPos.push_back((GameManager::instance->teamMoveOutOfHome == 0) ? 14 : 34);
		ShowIndicators(indPos);
	}
}

void HideIndicators()
{
	for (int x = 0; x < 40; x++)
	{
		GameObject::FindObject("indicator" + std::to_string(x))->isRendering = false;
	}
	GameObject::FindObject("indicatorlastpos")->isRendering = false;
}