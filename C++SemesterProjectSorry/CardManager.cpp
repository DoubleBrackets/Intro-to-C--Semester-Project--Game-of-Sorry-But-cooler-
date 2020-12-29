#include "CardManager.h"
#include "GameManager.h"
#include <iostream>


CardManager* CardManager::instance;


CardManager::CardManager()
{

	Card* testcard = new Card("test", 33, 10000);
	//AddCard(testcard, "test");
	//basic cards
	instance = this;
	std::vector<int> pos;
	for (int x = 1; x <= 10; x++)
	{
		Card* card0 = new Card("Move " +std::to_string(x) + ": Move " + std::to_string(x) + " squares forward" , x, 12);
		AddCard(card0,"move" + std::to_string(x));
		//pos.push_back(x);
	}

	pos.clear();
	pos = { 2 };
	Card* newcard = new Card("2x Hop: Hop forwards 2 squares, 2 times" ,pos, 4, 10);
	AddCard(newcard,"hop2");

	pos.clear();
	pos = { 2,4 };
	newcard = new Card("3x Hop: Hop forwards 2 squares, 3 times", pos, 6, 10);
	AddCard(newcard,"hop3");

	pos.clear();
	pos = { 1,2 };
	newcard = new Card("Dash: Move forwards 3 squares, knocking out any piece in the way", pos, 3, 10);
	AddCard(newcard,"dash3");

	pos.clear();
	pos = { 2 };
	newcard = new Card("Strikethrough: Move forwards 1 square, knocking out any piece 1 square further", pos, 1, 10);
	AddCard(newcard,"strikethrough");

	pos.clear();
	pos = { -1 };
	newcard = new Card("Afterburn: Knock out any piece 1 square behind, and Move forwards 5 squares", pos, 5, 10);
	AddCard(newcard, "afterburn");

	pos.clear();
	pos = { 1 };
	newcard = new Card("Reversal: Knock out any piece 1 square in front, and Move back 1 square", pos, -1, 10);
	AddCard(newcard, "reversal");

	pos.clear();
	pos = { 3,5 };
	newcard = new Card("AOE: Move forwards 4 squares, knocking out any piece in the adjacent 2 squares", pos, 4, 10);
	AddCard(newcard, "leap");

	pos.clear();
	pos = { -1,2,3 };
	newcard = new Card("Sweep: Move forwards 1 square, knocking out pieces in the adjacent 4 squares", pos, 1, 10);
	AddCard(newcard, "sweep");

	//pos.clear();
	//pos = { -1,2,3 };
	//newcard = new Card("Full Reverse(over a squirrel): Move  squares backwards, knocking out any piece in the path", pos, -3, 10);
	//AddCard(newcard, "sweep");

	pos.clear();
	for (int x = 0; x < 40; x++)
		pos.push_back(x);
	newcard = new Card("Relatively Minor Explosion: Destroy all pieces on the board (excluding safe zones)", pos, 0, 2);
	AddCard(newcard, "minorexplosion");
	//dynamic cards

	newcard = new Card("Bulldoze: Move forwards until the first piece hit, knocking that piece out", 3, 4);
	AddCard(newcard, "bulldoze");

	newcard = new Card("Not a Blue Shell: Move to and knock out the piece closest to its safe zone", 0, 4);
	newcard->targetBoardPosition = true;
	AddCard(newcard, "blueshell");

	newcard = new Card("Divide by 2: Move half the distance to the finish(rounding down)", 2, 2);
	//AddCard(newcard, "div2");
}

void CardManager::AddCard(Card* c,std::string name)
{
	cards.insert(std::pair<std::string, Card*>(name,c));
	totalWeight += c->weight;
}
Card* CardManager::GetRandomCard()
{
	int i = rand() % totalWeight;
	std::map<std::string, Card*>::iterator it;
	int t = 0;
	for (it = cards.begin(); it != cards.end(); it++)
	{
		t += it->second->weight;
		if (t > i)
		{
			return it->second;
		}
	}
	it = cards.begin();
	return it-> second;
}

Card* CardManager::GetCard(std::string name)
{
	return cards.find(name)->second;
}

void CardManager::UpdateActiveCards(piece* m,Card* c)//updates values for cards that have dynamic effects, based on selected piece
{
	//bulldoze
	Card* bulldoze = GetCard("bulldoze");
	if (c == bulldoze)
	{
		UpdateBulldoze(m, c);
		return;
	}
	//blue shell
	Card* bshell = GetCard("blueshell");
	if (c == bshell)
	{
		UpdateBlueshell(m, c);
		return;
	}
}


void CardManager::UpdateBlueshell(piece* m, Card* c)
{
	int space;
	piece** b = GameManager::instance->board;
	int targetpos = m->pos;

	for (int x = 0; x < 34; x++)
	{
		int redspace = GameManager::instance->MovePieceMain(-x, 7, -1);
		int bluespace = GameManager::instance->MovePieceMain(-x, 27, -1);
		if (b[redspace] != NULL && b[redspace]->team == 0)
		{
			targetpos = redspace;
			if(m->team == 1)
				break;
			x = 50;
		}
		if (b[bluespace] != NULL && b[bluespace]->team == 1)
		{
			targetpos = bluespace;
			break;
		}
	}

	std::cout << targetpos << std::endl;
	c->endingPos = targetpos;
}

void CardManager::UpdateBulldoze(piece* m, Card* bulldoze)
{
	int bulldozespaces = 2;
	for (int x = 1; x <= 34; x++)
	{
		int nPos = GameManager::instance->MovePieceMain(x, m->pos, m->team);
		if (nPos != -1)
		{
			if (nPos == -2)
				break;
			if (GameManager::instance->board[nPos] != NULL)
			{
				bulldozespaces = x;
				break;
			}
		}
	}
	bulldoze->endingPos = bulldozespaces;
}