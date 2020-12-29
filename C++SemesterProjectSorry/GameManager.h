#pragma once
#include "piece.h"
#include "CardManager.h"
class GameManager {
public:
	piece* board[40];                          //Pointer array
	std::vector <piece*> home0, home1;                   //Home vector declaration
	std::vector <piece*> end0, end1;                     //End vectors

	int currentTurn = 1;
	bool gameEnded = false;

	piece* r0, * r1, * r2, * r3, * b0, * b1, * b2, * b3;      //Pointers to pieces
	

	static GameManager* instance;

	CardManager* cardManager;
	bool canStartNextTurn = true;
	Card* currentCard;

	Card* team0StoredCard;
	Card* team1StoredCard;

	std::shared_ptr<TextObject> currentcardtext;
	std::shared_ptr<TextObject> storedcard0text;
	std::shared_ptr<TextObject> storedcard1text;
	std::shared_ptr<TextObject> turntext;

	std::shared_ptr<GameObject> home0sprite;
	std::shared_ptr<GameObject> home1sprite;

	std::shared_ptr<GameObject> finish0sprite;
	std::shared_ptr<GameObject> finish1sprite;

	piece* selectedPiece;

	int teamMoveOutOfHome = -1;

	GameManager();
	void InitGame();
	
	bool CheckForEnd();
	bool StartNewTurn();
	void EndTurn();

	void SetCurrentCard(Card*);

	bool CheckValidMoves(Card* c);

	bool SelectPieceForMove(piece*);
	bool CheckPieceForMove(piece*, Card*);
	bool MovePiece(int, piece*);
	int MovePieceMain(int spaces, int pos, int team);
	void ReturnPieceToHome(piece*);

	void SwapCard();
	std::vector<int> SelectPiece(piece*);
	piece* MovePieceOutOfHome(int);
	bool SelectHomePieceToMove(int);
	std::string PrintPiece(piece*);

};