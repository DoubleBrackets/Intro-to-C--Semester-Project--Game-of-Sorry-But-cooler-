#include "ResourceManager.h"
#include <SFML/Graphics.hpp>



std::map<std::string, sf::Texture> ResourceManager::textureMap;
ResourceManager::ResourceManager(std::string k)
{
	AddTexture("board_tex", "Assets/board.png");
	AddTexture("redpiece_tex", "Assets/RedPiece.png");
}

void ResourceManager::AddTexture(std::string key, std::string filepath)
{
	sf::Texture newTex;
	newTex.loadFromFile(filepath);
	textureMap.insert(std::pair<std::string, sf::Texture>(key, newTex));
}



