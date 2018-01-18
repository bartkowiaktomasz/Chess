#include "header/Bishop.h"

#include <iostream>
#include <string>

using namespace std;

Bishop::Bishop(Colour colour, string currentPosition): Piece(colour, currentPosition)
{
    if(colour == Colour::White)
        symbol_ = "♝";
    else
        symbol_ = "♗";
}
Bishop::~Bishop() {}

bool Bishop::isValidMove(string nextPosition, map<string, Piece*> Board, int& errorCode)
{
    if(isClearDiagonal(currentPosition_, nextPosition, Board))
    {
        return true;
    } else
    {
        errorCode = ERROR_INVALID_BISHOP_MOVE;
        return false;
    }
}
