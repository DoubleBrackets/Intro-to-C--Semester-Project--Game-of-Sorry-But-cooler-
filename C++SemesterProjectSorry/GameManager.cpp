// sorry2D.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include "piece.h"
#include "GameManager.h"

GameManager* GameManager::instance;

GameManager::GameManager()
{
    instance = this;
}
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

    home1.push_back(b0 = new piece(-1, 1,0));
    home1.push_back(b1 = new piece(-1, 1,1));
    home1.push_back(b2 = new piece(-1, 1,2));
    home1.push_back(b3 = new piece(-1, 1,3));


    cardManager = new CardManager();

}

bool GameManager::StartNewTurn()//updates visuals and starts new turn
{
    //Checks if game has ended
    if (CheckForEnd())
    {
        gameEnded = true;
        return false;
    }
    if (!canStartNextTurn)//turn is currently ongoing
        return true;

    canStartNextTurn = false;
    r0->UpdatePosition();
    r1->UpdatePosition();
    r2->UpdatePosition();
    r3->UpdatePosition();
    b0->UpdatePosition();
    b1->UpdatePosition();
    b2->UpdatePosition();
    b3->UpdatePosition();

    currentTurn = (currentTurn + 1) % 2;

    //Visuals updates
    turntext->SetText((currentTurn == 0) ? "Red Turn" : "Blue Turn");
    turntext->SetColor((currentTurn == 0) ? sf::Color::Red : sf::Color::Blue);

    home0sprite->StartAnimation(std::to_string(home0.size()));
    home1sprite->StartAnimation(std::to_string(home1.size()));

    finish0sprite->StartAnimation(std::to_string(end0.size()));
    finish1sprite->StartAnimation(std::to_string(end1.size()));
    //Draw card
    SetCurrentCard( cardManager->GetRandomCard());

    std::cout << "Card drawn: " << currentCard->description << std::endl;

    //checks for valid moves
    bool storedCardHasValid = false;
    if (currentTurn == 0 && team0StoredCard != NULL)
        storedCardHasValid = CheckValidMoves(team0StoredCard);
    else if(currentTurn == 1 && team1StoredCard != NULL)
        storedCardHasValid = CheckValidMoves(team1StoredCard);
    bool currentCardHasValid = CheckValidMoves(currentCard);
    if (!storedCardHasValid && !currentCardHasValid)
    {
        std::cout << "No valid moves, ending turn" << std::endl;
        EndTurn();
    }
    return true;
}

bool GameManager::CheckValidMoves(Card* c)
{
    //Checks to see if moves are valid for each piece
    bool anyValidMoves = false;
    if (currentTurn == 0)
    {
        r0->hasValidMoves = CheckPieceForMove(r0, c);
        r1->hasValidMoves = CheckPieceForMove(r1,c);
        r2->hasValidMoves = CheckPieceForMove(r2, c);
        r3->hasValidMoves = CheckPieceForMove(r3, c);
        if (!(r0->hasValidMoves || r1->hasValidMoves || r2->hasValidMoves || r3->hasValidMoves || (home0.size() != 0 && (board[14] == NULL || board[14]->team != 0))))//no valid moves, end turn
        {
            return false;
        }
    }
    else
    {
        b0->hasValidMoves = CheckPieceForMove(b0, c);
        b1->hasValidMoves = CheckPieceForMove(b1,c);
        b2->hasValidMoves = CheckPieceForMove(b2, c);
        b3->hasValidMoves = CheckPieceForMove(b3, c);
        if (!(b0->hasValidMoves || b1->hasValidMoves || b2->hasValidMoves || b3->hasValidMoves || (home1.size() != 0 && (board[34] == NULL || board[34]->team != 1))))//no valid moves, end turn
        {
            return false;
        }
    }
    return true;
}

void GameManager::SwapCard()
{
    if (currentTurn == 0)
    {
        if (team0StoredCard == NULL)//store card and end turn
        {
            std::cout << "Card stored to Red Team: " << currentCard->description << std::endl;
            team0StoredCard = currentCard;
            SetCurrentCard(NULL);
            EndTurn();
        }
        else//otherwise, swap cards
        {
            std::cout << "Card swapped into play: " << team0StoredCard->description << std::endl;
            Card* temp = team0StoredCard;
            team0StoredCard = currentCard;
            SetCurrentCard(temp);
            CheckValidMoves(currentCard);
        }
    }
    else
    {
        if (team1StoredCard == NULL)//store card and end turn
        {
            std::cout << "Card stored to Blue Team: " << currentCard->description << std::endl;
            team1StoredCard = currentCard;
            SetCurrentCard(NULL);
            EndTurn();
        }
        else//otherwise, swap cards
        {
            std::cout << "Card swapped into play: " << team1StoredCard->description << std::endl;
            Card* temp = team1StoredCard;
            team1StoredCard = currentCard;
            SetCurrentCard(temp);
            CheckValidMoves(currentCard);
        }
    }
    storedcard0text->SetText((team0StoredCard == NULL) ? "" : team0StoredCard->description);
    storedcard1text->SetText((team1StoredCard == NULL) ? "" : team1StoredCard->description);
}

void GameManager::SetCurrentCard(Card* c)
{
    currentCard = c;
    currentcardtext->SetText((c==NULL)?"":c->description);
}

bool GameManager::CheckForEnd()
{
    if (end0.size() == 4)
    {
        std::cout << "Red has won!" << std::endl;
        return true;
    }
    else if (end1.size() == 4)
    {
        std::cout << "Blue has won!" << std::endl;
        return true;
    }
    return false;
}

void GameManager::EndTurn()//finishes turn and resets values to have StartNewTurn run on the next game tick
{
    canStartNextTurn = true;
    selectedPiece = NULL;
    teamMoveOutOfHome = -1;
    std::cout << "PIECES IN HOME: R " << home0.size() << " B " << home1.size() << std::endl;
    std::cout << "PIECE FINISHED: R " << end0.size() << " B " << end1.size() << std::endl;

    for (int x = 0; x < 40; x++)
    {
        if (board[x] == NULL)
            std::cout << "-";
        else
            std::cout << board[x]->team;
    }
    std::cout << std::endl;
}

std::vector<int> GameManager::SelectPiece(piece* m)
{

    int pos = m->pos;
    std::vector<int> positions;
    if (!m->hasValidMoves)
        return positions;

    int newPos;
    if (!currentCard->targetBoardPosition)
        newPos = MovePieceMain(currentCard->endingPos, m->pos, m->team);
    else
        newPos = currentCard->endingPos;

    selectedPiece = m;

    //does not apply sorry positions if in safe zone
    if (!((pos > 7 && pos < 13) || (pos > 27 && pos < 33)))
    {
        std::vector<int> cardpositions = currentCard->positions;
        //sends all pieces within the positions back to home
        int l = cardpositions.size();
        for (int x = 0; x < l; x++)
        {
            int checkPos = MovePieceMain(cardpositions[x], pos, -1);
            positions.push_back(checkPos);
        }
    }
    positions.push_back(newPos);
    return positions;
}

bool GameManager::CheckPieceForMove(piece* piece, Card* card)//checks if piece can move using card
{
    if (piece == NULL)
        return false;
    if (card->targetBoardPosition)
    {
        int pos = piece->pos;
        if ((pos > 7 && pos < 13) || (pos > 27 && pos < 33))//targetting doesnt apply when in safe zone
            return false;
        return true;
    }
    return MovePieceMain(card->endingPos, piece->pos, piece->team) != -1;
}

bool GameManager::SelectPieceForMove(piece* m)//Attempts to move piece using  current card
{
    int pos = m->pos;

    //Checks if it is on correct team and has valid move
    if (m->team != currentTurn || !m->hasValidMoves)
        return false;

    //does not apply sorry positions if in safe zone
    if (!((pos > 7 && pos < 13) || (pos > 27 && pos < 33)))
    {
        std::vector<int> positions = currentCard->positions;
        //sends all pieces within the positions in vector back to home
        int l = positions.size();
        for (int x = 0; x < l; x++)
        {
            int checkPos = MovePieceMain(positions[x], pos, -1);
            if (checkPos != -1 && board[checkPos] != NULL)
            {
                ReturnPieceToHome(board[checkPos]);
            }
        }
    }

    if (currentCard->targetBoardPosition)
    {
        int pos = currentCard->endingPos;
        piece* toko = board[pos];
        if (board[pos] != NULL)
        {
            ReturnPieceToHome(toko);
        }
        if (toko != m)
        {
            board[m->pos] = NULL;
            board[pos] = m;
            m->pos = pos;
        }
    }
    else
    {
        MovePiece(currentCard->endingPos, m);
    }

    EndTurn();
    return true;
}

bool GameManager::SelectHomePieceToMove(int team)//attempts to move a  new piece out of home, using the current card
{
    if (team != currentTurn)
        return false;
    piece* newPiece;
    if (team == 0)
    {
        if (home0.size() == 0)// || !MovePieceMain(currentCard, 14, 0))
            return false;
        newPiece = MovePieceOutOfHome(0);
    }
    else
    {
        if (home1.size() == 0)// || !MovePieceMain(currentCard,34,1))
            return false;
        newPiece = MovePieceOutOfHome(1);

    }
    if (newPiece == NULL)
        return false;
    std::cout << PrintPiece(newPiece) << " has been moved onto the board!" << std::endl;
    std::string anim = ((team == 0) ? "red" : "blue");
    anim += "summonanim";
    GameObject::FindObject(anim)->StartAnimation("default");
    //after moving out of home, move the piece using the card
    //SelectPieceForMove(newPiece);
    EndTurn();
}


bool GameManager::MovePiece(int spaces,piece* m)//Attempts to move piece a number of spaces
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
        std::cout << PrintPiece(m) << " has reached the end! " << std::endl;
        std::string anim = ((m->team == 0) ? "red" : "blue");
        anim += "finishanim";
        GameObject::FindObject(anim)->StartAnimation("default");
    }
    else if (board[fPos] != NULL)//existing piece on the target square
    {
        piece* displacedPiece = board[fPos];
        //bump piece
        ReturnPieceToHome(displacedPiece);
        board[m->pos] = NULL;
        if (displacedPiece != m)
        {
            m->pos = fPos;
            board[fPos] = m;
        }
    }
    else//move to new square
    {
        std::cout << PrintPiece(m) << " moved to " << fPos << std::endl;
        board[m->pos] = NULL;
        m->pos = fPos;
        board[fPos] = m;
    }
    return true;
}

int GameManager::MovePieceMain(int spaces, int currentPos, int team) //finds position of piece after moving, returns new location or -1 if invalid move, -2 if reaches end
{         
    if (currentPos == -1)
        return -1;
    if (currentPos + spaces > 7 && currentPos < 13 && spaces > 0)   //red ladder
    {
        if(team != 0)//wrong team, move past
            return MovePieceMain(spaces-(7-currentPos)-1, 13,team);
        else
        {
            if (currentPos + spaces == 13)//reaches the end
                return -2;
            if (currentPos + spaces > 13)//invalid move, since it would take you over the end
                return -1;
            else if (board[currentPos + spaces] != NULL)//cannot bump own team in safe zone
                return -1;
            return currentPos + spaces;//move into safe zone
        }
    }
    else if (currentPos + spaces > 27 && currentPos < 33 && spaces > 0) //blue ladder
    {
        if(team != 1)//wrong team, move past
            return MovePieceMain(spaces - (27-currentPos) - 1, 33, team);
        else
        {
            if (currentPos + spaces == 33)//reaches the end
                return -2;
            else if (currentPos + spaces > 33)//invalid move, since it would take you over the end
                return -1;
            else if (board[currentPos + spaces] != NULL)//cannot bump own team in safe zone
                return -1;
            return currentPos + spaces;//move into safe zone
        }
    }
    else if (currentPos + spaces < 13 && currentPos >= 13)//reversing past red ladder
    {
        return MovePieceMain(spaces - (13 - currentPos) + 1, 7, team);
    }
    else if (currentPos + spaces < 33 && currentPos >= 33)//reversing past blue ladder
    {
        return MovePieceMain(spaces - (33 - currentPos) + 1, 27, team);
    }
    else if (currentPos + spaces > 39)                                     // loops position to the right if goes past the edge
    {
        return MovePieceMain(spaces - (39 - currentPos) - 1, 0,team);
    }
    else if (currentPos + spaces < 0)//loops positions to the left
    {
        return MovePieceMain(spaces + currentPos + 1, 39, team);
    }
    else//valid, normal move
    {
        //if (board[currentPos + spaces] != NULL && board[currentPos + spaces]->team == team)//cannot bump own team
            //return -1;
        return (currentPos + spaces);
    }
}

void GameManager::ReturnPieceToHome(piece* m)
{
    std::cout << PrintPiece(m) << " was bumped at " << m->pos << std::endl;
    //removes from board
    board[m->pos] = NULL;
    m->pos = -1;
    //re-adds to home vector
    if (m->team == 0)
    {
        home0.push_back(m);
    }
    else
        home1.push_back(m);
}

piece* GameManager::MovePieceOutOfHome(int team)//moves piece out of home
{
    if (team == 0)
    {
        if (home0.size() == 0)
            return  NULL;
        if (board[14] != NULL)
        {
            if (board[14]->team == team)
                return NULL;
            else//knock out the opponent piece
            {
                std::cout << PrintPiece(home0[0]) << " bumped " << PrintPiece(board[14]) << " at " << "14" << std::endl;
                ReturnPieceToHome(board[14]);
            }
        }
        piece* m = home0[0];
        home0.erase(home0.begin());
        //add onto main board
        
        board[14] = m;
        m->pos = 14;
        return m;
    }
    else
    {
        if (home1.size() == 0)
            return NULL;
        if (board[34] != NULL)
        {
            if (board[34]->team == team)
                return NULL;
            else//knock out the opponent piece
            {
                std::cout << PrintPiece(home1[0]) << " bumped " << PrintPiece(board[34]) << " at " << "34" << std::endl;
                ReturnPieceToHome(board[34]);
            }
        }
        piece* m = home1[0];
        home1.erase(home1.begin());
        //add onto main board
        board[34] = m;
        m->pos = 34;
        return m;
    }
    return NULL;
}

std::string GameManager::PrintPiece(piece* m)
{
    return ("piece " + std::to_string(m->piecenumber) + " of team " + std::to_string(m->team));
}





