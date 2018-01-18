#include "header/Queen.h"

#include <iostream>
#include <string>

using namespace std;

Queen::Queen(Colour colour, string currentPosition): Piece(colour, currentPosition)
{
    if(colour == Colour::White)
        symbol_ = "♛";
    else
        symbol_ = "♕";
}
Queen::~Queen() {}

bool Queen::isValidMove(string nextPosition, map<string, Piece*> Board, int& errorCode)
{
    if(isClearRank(currentPosition_, nextPosition, Board) ||
       isClearFile(currentPosition_, nextPosition, Board) ||
       isClearDiagonal(currentPosition_, nextPosition, Board))
    {
        return true;
    } else
    {
        errorCode = ERROR_INVALID_QUEEN_MOVE;
        return false;
    }
}
