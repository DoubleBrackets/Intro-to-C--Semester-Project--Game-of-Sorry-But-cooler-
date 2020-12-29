#include "GameObject.h"
#include "ResourceManager.h"

TextObject::TextObject(std::string name) : GameObject::GameObject(name)
{
	mainText.setFont(ResourceManager::font);
	mainText.setFillColor(sf::Color::Black);
	hasText = true;
	mainText.setScale(sf::Vector2f(0.5, 0.5));
}

void TextObject::SetPosition(sf::Vector2f pos)
{
	GameObject::SetPosition(pos);
	mainText.setPosition(pos);
}

void TextObject::SetWraparound(int size)
{
	wraparoundCount = size;
	SetText(rawText);
}

void TextObject::SetScale(double scale)
{
	mainText.setScale(sf::Vector2f(scale, scale));
}

void TextObject::SetText(std::string str)
{
	rawText = str;
	int l = str.length();
	//wraparound
	int lastspaceindex = -1;
	int c = 1;
	for (int x = 0; x < l; x++)
	{
		if (str[x] == ' ')
		{
			lastspaceindex = x;
		}
		if (c > wraparoundCount)
		{
			if (lastspaceindex != -1)
			{
				str.insert(lastspaceindex + 1, "\n");
				c = x - lastspaceindex;
			}
			else
			{
				str.insert(x, "\n");
				lastspaceindex = -1;
				c = 1;
			}
			l++;
			x++;
		}
		c++;
	}
	//int num = l / wraparoundCount;
	//for (int x = 1; x <= num; x++)
	//{
	//	str.insert(x*wraparoundCount+2*(x-1), "\n");
	//}
	mainText.setString(str);
}

void TextObject::SetColor(sf::Color color)
{
	mainText.setFillColor(color);
}