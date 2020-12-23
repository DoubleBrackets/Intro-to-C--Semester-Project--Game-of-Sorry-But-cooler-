#pragma once
#include "GameObject.h"
class piece {
public:
    int pos;
    int team;
    int piecenumber;

    std::shared_ptr<PlayerPiece> pieceObject;

    piece(int p, int t,int n) {
        pos = p;
        team = t;
        piecenumber = n;
        //creates graphics object for piece
        pieceObject = std::make_shared<PlayerPiece>(team, team+"piece"+piecenumber,this);
        pieceObject->InitializeObject(pieceObject);
        pieceObject->isRendering = false;
    }
    void UpdatePosition() {
        if (pos != -1)
            pieceObject->isRendering = true;
        pieceObject->UpdatePosition(pos);
    }
};