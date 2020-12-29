#pragma once
#include <string>
#include <vector>
#include "GameObject.h";
struct Card
{
	std::string description;
	std::vector<int> positions;//positions where pieces are returned to home
	int endingPos;
	int weight;
	bool targetBoardPosition = false;//target position is a specfic board position, rather than a relative position to the piece
	Card(std::string description, std::vector<int> positions,int endingPos,int weight)
	{
		this->description = description;
		this->positions = positions;
		this->endingPos = endingPos;
		this->weight = weight;
	}
	Card(std::string description, int endingPos, int weight)
	{
		this->description = description;
		this->endingPos = endingPos;
		this->weight = weight;
	}
};
class CardManager {
public:
	std::map<std::string, Card*> cards;

	static CardManager* instance;
	int totalWeight = 0;
	CardManager();
	void AddCard(Card*,std::string);

	void UpdateActiveCards(piece*, Card* c);

	Card* GetRandomCard();
	Card* GetCard(std::string);

	void UpdateBulldoze(piece*, Card*);
	void UpdateBlueshell(piece*, Card*);
};

