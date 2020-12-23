#pragma once
#include "piece.h"
class GameManager {
public:
	void InitGame();
	void ConsoleDraw();
	bool MovePiece(int, piece*);
	int MovePieceMain(int, int, int);
	void ReturnPieceToHome(piece*);
	bool MovePieceOutOfHome(piece* m);
	int GetCard();

	piece* board[40];                          //Pointer array
	std::vector <piece*> home0, home1;                   //Home vector declaration
	std::vector <piece*> end0, end1;                     //End vectors

	int currentTurn = 0;


	piece* r0, * r1, * r2, * r3, * b0, * b1, * b2, * b3;      //Pointers
};