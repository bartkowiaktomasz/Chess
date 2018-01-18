#include "header/Knight.h"

#include <iostream>
#include <string>
#include <cmath>

using namespace std;

Knight::Knight(Colour colour, string currentPosition): Piece(colour, currentPosition)
{
    if(colour == Colour::White)
        symbol_ = "♞";
    else
        symbol_ = "♘";
}
Knight::~Knight() {}

bool Knight::isValidMove(string nextPosition, map<string, Piece*> Board, int& errorCode)
{
    if((abs(nextPosition[0] - currentPosition_[0]) == 2 &&
        abs(nextPosition[1] - currentPosition_[1]) == 1) ||
       (abs(nextPosition[1] - currentPosition_[1]) == 2 &&
        abs(nextPosition[0] - currentPosition_[0]) == 1))
    {
        return true;
    } else
    {
        errorCode = ERROR_INVALID_KNIGHT_MOVE;
        return false;
    }
}
