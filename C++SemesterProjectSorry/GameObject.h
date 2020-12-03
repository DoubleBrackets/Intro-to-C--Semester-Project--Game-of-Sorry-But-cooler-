#pragma once
#include<vector>
#include <SFML/Graphics.hpp>
class GameObject
{
public:
	sf::Vector2f position;
	std::string objectName;
	static std::vector<GameObject> spriteList;
	sf::Sprite currentSprite;
	bool isRendering = false;

	GameObject(std::string,std::string,int,int);
	GameObject(std::string,std::string, int, int,int);
	void SetPosition(sf::Vector2f);
	void AddSpriteToRender();
	void RemoveSpriteFromRender();
	static GameObject *FindObject(std::string);
};

