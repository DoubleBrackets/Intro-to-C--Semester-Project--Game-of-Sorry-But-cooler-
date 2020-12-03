#include "ResourceManager.h"
#include <SFML/Graphics.hpp>



std::map<std::string, sf::Texture> ResourceManager::textureMap;
std::map<std::string, Animation> ResourceManager::animationMap;
ResourceManager::ResourceManager(std::string k)
{
	AddTexture("board", "Assets/board.png");
}

void ResourceManager::AddTexture(std::string key, std::string filepath)
{
	sf::Texture newTex;
	newTex.loadFromFile(filepath);
	textureMap.insert(std::pair<std::string, sf::Texture>(key, newTex));
}

void ResourceManager::AddAnimation(std::string key, std::string filepath, int frames, sf::Vector2f size)
{
	Animation newAnim;
	sf::Texture newTex;
	newTex.loadFromFile(filepath);
	//newAnim.frames.push_back();
}



