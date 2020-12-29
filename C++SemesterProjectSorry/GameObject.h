#pragma once
#include<vector>
#include<chrono>
#include<iostream>
#include <SFML/Graphics.hpp>
class piece;
struct Animation 
{
	//Stores frames as IntRect, for manipulation using sprite.TextureRect
	sf::IntRect frames[20];
	int frameCount;
	double frameInterval;
	bool looping = true;
	std::string name;
	std::chrono::high_resolution_clock::time_point startTime;
};

class GameObject
{
public:
	sf::Vector2f position;
	sf::Vector2f velocity;

	std::string objectName;
	static std::vector<std::shared_ptr<GameObject>> spriteList;
	std::vector<Animation> animationList;
	sf::Sprite currentSprite;

	bool clickable = false;
	bool isRendering = true;
	bool isInitialized = false;
	bool animationPlaying = true;

	bool hasText = false;
	sf::Text mainText;

	int currentAnimationIndex= -1;

	GameObject(std::string);
	void virtual SetPosition(sf::Vector2f);
	void MoveObject(sf::Vector2f);
	void InitializeObject(std::shared_ptr<GameObject>);
	void DeleteObject();
	void SetTexture(std::string,double);
	static std::shared_ptr<GameObject> FindObject(std::string);
	void AddAnimation(std::string, int,double,sf::Vector2f,int,int,sf::Vector2f,bool);
	void SetAnimationFrame(int);
	void StartAnimation(std::string);
	void StopAnimation();
	void UpdateGameObject(long,long);
	void FlipSprite();
	bool CheckForClick(sf::Vector2f,bool);
	virtual void OnClick() {};
	virtual void OnRelease(bool) {};

	static long ConvertToMilli(std::chrono::high_resolution_clock::time_point);
};

class TextObject : public GameObject
{
public:

	int wraparoundCount = 1000;

	std::string rawText;

	TextObject(std::string);

	void SetPosition(sf::Vector2f) override;
	void SetWraparound(int);
	void SetText(std::string);
	void SetColor(sf::Color);
	void SetScale(double scale);
};

class Clickable : public GameObject
{
public:
	Clickable(std::string);
	void OnClick() override;
	void OnRelease(bool) override;
};
 
class PlayerPiece : public Clickable { 
public:
	piece* gamePiece;
	PlayerPiece(int, std::string,piece*);
	void UpdatePosition(int);
	void OnRelease(bool) override;
	void ShowCardIndicators();
};

class TeamHome : public Clickable {//clickable allowing player to bring a new piece onto the board during a move
public:
	TeamHome(std::string, int);
	int team;
	void OnRelease(bool) override;
};

class ConfirmButton : public Clickable
{
public:
	ConfirmButton(std::string);
	void OnRelease(bool) override;
	static void CreateIndicators();
};


class SwapButton : public Clickable
{
public:
	SwapButton(std::string);
	void OnRelease(bool) override;
};

