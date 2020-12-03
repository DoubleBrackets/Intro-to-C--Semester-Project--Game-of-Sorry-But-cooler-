#pragma once
#include <map>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <list>

class ResourceManager
{
public:
	static std::map<std::string, sf::Texture> textureMap;
	ResourceManager(std::string);
	void AddTexture(std::string, std::string);
};

