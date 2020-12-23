// sorry2D.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include "piece.h"
#include "GameManager.h"


//team 0 is red, team 1 is blue
void GameManager::InitGame() 
{
    for (int i = 0; i < 40; i++) 
    {
         board[i] = NULL;
    }

    home0.push_back(r0 = new piece(-1, 0,0));  
    home0.push_back(r1 = new piece(-1, 0,1));
    home0.push_back(r2 = new piece(-1, 0,2));
    home0.push_back(r3 = new piece(-1, 0,3));
    MovePieceOutOfHome(r0);
    r0->UpdatePosition();
    home1.push_back(b0 = new piece(-1, 1,0));
    home1.push_back(b1 = new piece(-1, 1,1));
    home1.push_back(b2 = new piece(-1, 1,2));
    home1.push_back(b3 = new piece(-1, 1,3));
    MovePieceOutOfHome(b0);
    b0->UpdatePosition();
}

void GameManager:: ConsoleDraw() {
    for (int i = 0; i < 12; i++) {                                     
        std::cout << "| ";
        if (board[i] != NULL ) {
            std::cout << ". |";
        }
        else 
            std::cout << "|";
    }

    for (unsigned int i = 0; i < end0.size(); i++) {
        std::cout << ".";
    }

    if (board[13] != NULL) 
        std::cout << ". |";
    else
        std::cout << "|";
    
    for (unsigned int i = 0; i < home0.size(); i++) {
        std::cout << ".";
    }

    for (int i = 15; i < 32; i++) {
        if (board[i] != NULL) {
            std::cout << ". |";
        }
        else
            std::cout << "|";
    }

    for (unsigned int i = 0; i < end1.size(); i++) {
        std::cout << ".";
    }

    if (board[33] != NULL)
        std::cout << ". |";
    else
        std::cout << "|";

    for (unsigned int i = 0; i < home1.size(); i++) {
        std::cout << ".";
    }

    for (int i = 35; i < 40; i++) {
        if (board[i] != NULL) {
            std::cout << ". |";
        }
        else
            std::cout << "|";
    }
}

bool GameManager::MovePiece(int spaces,piece* m)
{
    int fPos = MovePieceMain(spaces, m->pos, m->team);
    if (fPos == -1)//invalid move
    {
        return false;
    }
    else if (fPos == -2)//reach end
    {
        board[m->pos] = NULL;
        m->pos = -1;
        if (m->team == 0)
            end0.push_back(m);
        else
            end1.push_back(m);
    }
    else if (board[fPos] != NULL)//existing piece on the target square
    {
        piece* displacedPiece = board[fPos];
        if (displacedPiece->team == m->team)//invalid, cannot bump own piece
            return false;
        else//bump piece
        {
            ReturnPieceToHome(displacedPiece);
            m->pos = fPos;
            board[fPos] = m;
        }
    }
    else//move to new square
    {
        board[m->pos] = NULL;
        m->pos = fPos;
        board[fPos] = m;
    }
    return true;
}

int GameManager::MovePieceMain(int spaces, int currentPos, int team) //attempts to move piece, returns new location or -1 if invalid move, -2 if reaches end
{                
    if (currentPos + spaces > 7 && currentPos <= 7)   //red ladder
    {
        if(team == 1)//wrong team, move past
            return MovePieceMain(spaces-(currentPos-7)-1, 13,team);
        else
        {
            if (currentPos + spaces == 13)//reaches the end
                return -2;
            else if (currentPos + spaces > 13)//invalid move, since it would take you over the end
                return -1;
            return currentPos + spaces;//move into safe zone
        }
    }
    else if (currentPos + spaces > 27 && currentPos <= 27) //blue ladder
    {
        if(team == 0)//wrong team, move past
            return MovePieceMain(spaces - (currentPos - 27) - 1, 33, team);
        else
        {
            if (currentPos + spaces == 33)//reaches the end
                return -2;
            else if (currentPos + spaces > 33)//invalid move, since it would take you over the end
                return -1;
            return currentPos + spaces;//move into safe zone
        }
    }
    else if (currentPos + spaces > 39)                                     // loops position if goes past the edge
    {
        return MovePieceMain(spaces - (39 - currentPos) - 1, 0,team);
    }
    else//valid, normal move
        return (currentPos + spaces);
}

void GameManager::ReturnPieceToHome(piece* m)
{
    //removes from board
    board[m->pos] = NULL;
    m->pos = -1;
    //readds to home vector
    if (m->team == 0)
    {
        home0.push_back(m);
    }
    else
        home1.push_back(m);
}

bool GameManager::MovePieceOutOfHome(piece* m)
{
    if (m == NULL)
        return false;
    //Remove from home vector
    std::vector<piece*>::iterator it;

    if (m->team == 0)
    {
        if (home0.size() == 0)
            return false;
        for (it = home0.begin(); it != home0.end(); it++)
        {
            if (home0[it - home0.begin()] == m)
            {
                home0.erase(it);
                break;
            }
        }
        //add onto main board
        board[14] = m;
        m->pos = 14;
    }
    else
    {
        if (home1.size() == 0)
            return false;
        for (it = home1.begin(); it != home1.end(); it++)
        {
            if (home1[it - home1.begin()] == m)
            {
                home1.erase(it);
                break;
            }
        }
        //add onto main board
        board[34] = m;
        m->pos = 34;
    }
    return true;
}

int GameManager::GetCard() {                                 //Random Card picking method             
    int i = rand() % 45;

    if (i >= 0 && i <= 4)
        return 1;
    else if (i >= 5 && i <= 8)
        return 2;
    else if (i >= 9 && i <= 12)
        return 3;
    else if (i >= 13 && i <= 16)
        return 4;
    else if (i >= 17 && i <= 20)
        return 5;
    else if (i >= 21 && i <= 24)
        return 7;
    else if (i >= 25 && i <= 28)
        return 8;
    else if (i >= 29 && i <= 32)
        return 10;
    else if (i >= 33 && i <= 36)
        return 11;
    else if (i >= 37 && i <= 40)
        return 12;
    else if (i >= 41 && i <= 44)
        return 0;
}





