#pragma once
#include <map>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <list>

struct Animation{
	//std::list<sf::Texture> frames;
};
class ResourceManager
{
public:
	static std::map<std::string, sf::Texture> textureMap;
	static std::map<std::string, Animation> animationMap;
	ResourceManager(std::string);
	void AddTexture(std::string, std::string);
	void AddAnimation(std::string, std::string, int, sf::Vector2f);
};

