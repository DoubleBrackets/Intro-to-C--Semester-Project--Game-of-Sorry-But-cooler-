#pragma once
#include<vector>
#include <SFML/Graphics.hpp>
struct Animation 
{
	//Stores frames as IntRect, for manipulation using sprite.TextureRect
	sf::IntRect frames[20];
	int frameCount;
	double frameInterval;
	std::string name;
	double startTime;
};

class GameObject
{
public:
	sf::Vector2f position;
	std::string objectName;
	static std::vector<GameObject> spriteList;
	std::vector<Animation> animationList;
	sf::Sprite currentSprite;
	bool isRendering = false;
	int currentAnimationIndex= -1;

	GameObject(std::string);
	void SetPosition(sf::Vector2f);
	void InitializeToRenderList();
	void RemoveSpriteFromRender();
	void SetTexture(std::string,int);
	static GameObject *FindObject(std::string);
	void AddAnimation(std::string, int,double,sf::Vector2f,int,int,sf::Vector2f);
	void SetSpriteToFirstFrameOfAnimation(std::string);
	void StartAnimation(std::string);
	void StopAnimation();
};

