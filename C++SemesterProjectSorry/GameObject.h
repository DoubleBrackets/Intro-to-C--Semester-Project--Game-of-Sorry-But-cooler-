#pragma once
#include<vector>
#include<chrono>
#include <SFML/Graphics.hpp>

struct Animation 
{
	//Stores frames as IntRect, for manipulation using sprite.TextureRect
	sf::IntRect frames[20];
	int frameCount;
	double frameInterval;
	std::string name;
	std::chrono::high_resolution_clock::time_point startTime;
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
	void SetAnimationFrame(int);
	void StartAnimation(std::string);
	void StopAnimation();
	void UpdateGameObject(long);
	void FlipSprite();
	static long ConvertToMilli(std::chrono::high_resolution_clock::time_point);
};

