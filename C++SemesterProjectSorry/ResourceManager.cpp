#include "ResourceManager.h"
#include <SFML/Graphics.hpp>



std::map<std::string, sf::Texture> ResourceManager::textureMap;
sf::Font ResourceManager::font;
ResourceManager::ResourceManager(std::string k)
{
	AddTexture("board_tex", "Assets/board.png");
	AddTexture("redpiece_tex", "Assets/RedPiece.png");
	AddTexture("bluepiece_tex", "Assets/BluePiece.png");
	AddTexture("redhome_tex", "Assets/redhome.png");
	AddTexture("bluehome_tex", "Assets/bluehome.png");
	AddTexture("selectbox_tex", "Assets/selectbox.png");
	AddTexture("confirmbutton_tex", "Assets/confirmbutton.png");
	AddTexture("swapbutton_tex", "Assets/swapbutton.png");
	AddTexture("selectboxlastpos_tex", "Assets/selectboxlastpos.png");
	AddTexture("summonpiecered_tex", "Assets/summonpiecered.png");
	AddTexture("summonpieceblue_tex", "Assets/summonpieceblue.png");

	if (!font.loadFromFile("Assets/mainfont.ttf"))
	{
		std::cout << "Font failed to load" << std::endl;
	}

}

void ResourceManager::AddTexture(std::string key, std::string filepath)
{
	sf::Texture newTex;
	newTex.loadFromFile(filepath);
	textureMap.insert(std::pair<std::string, sf::Texture>(key, newTex));
}



