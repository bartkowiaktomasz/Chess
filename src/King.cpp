#include "header/King.h"

#include <iostream>
#include <string>
#include <cmath>

using namespace std;

King::King(Colour colour, string currentPosition): Piece(colour, currentPosition)
{
    isFirstMove_ = true;
    if(colour == Colour::White)
        symbol_ = "♚";
    else
        symbol_ = "♔";
}
King::~King() {}

bool King::isValidMove(string nextPosition, map<string, Piece*> Board, int& errorCode)
{
    if((abs(nextPosition[0] - currentPosition_[0]) == 1 &&
        abs(nextPosition[1] - currentPosition_[1]) <= 1) ||
       (abs(nextPosition[0] - currentPosition_[0]) == 0 &&
        abs(nextPosition[1] - currentPosition_[1]) <= 1))
    {
        return true;
    } else
    {
        errorCode = ERROR_INVALID_KING_MOVE;
        return false;
    }
}

bool King::isFirstMove()
{
    return isFirstMove_;
}

void King::setFirstMove(bool value)
{
    isFirstMove_ = false;
}
