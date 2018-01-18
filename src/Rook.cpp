#include "header/Rook.h"

#include <iostream>
#include <string>

using namespace std;

Rook::Rook(Colour colour, string currentPosition): Piece(colour, currentPosition)
{
    isFirstMove_ = true;
    if(colour == Colour::White)
        symbol_ = "♜";
    else
        symbol_ = "♖";
}
Rook::~Rook() {}

bool Rook::isValidMove(string nextPosition, map<string, Piece*> Board, int& errorCode)
{
    if(isClearRank(currentPosition_, nextPosition, Board) ||
       isClearFile(currentPosition_, nextPosition, Board))
    {
        return true;
    } else
    {
        errorCode = ERROR_INVALID_ROOK_MOVE;
        return false;
    }
}

bool Rook::isFirstMove()
{
    return isFirstMove_;
}

void Rook::setFirstMove(bool value)
{
    isFirstMove_ = false;
}
