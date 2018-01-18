#include "header/Pawn.h"

#include <iostream>
#include <string>

using namespace std;

Pawn::Pawn(Colour colour, string currentPosition): Piece(colour, currentPosition)
{
    isFirstMove_ = true;
    if(colour == Colour::White)
        symbol_ = "♟";
    else
        symbol_ = "♙";
}
Pawn::~Pawn() {}

bool Pawn::canMoveDoubleForward(string nextPosition, map<string, Piece*> Board, int sign)
{
    string middlePosition = nextPosition;
    middlePosition[1] -= sign;
    if(isFirstMove_ && (nextPosition[0] == currentPosition_[0]) &&
      (nextPosition[1] == currentPosition_[1] + 2*sign) &&
       Board[middlePosition] == nullptr && Board[nextPosition] == nullptr &&
       Board["" + nextPosition[0] + (nextPosition[1] - sign)] == nullptr)
   {
        return true;
   } else
        return false;
}

bool Pawn::canMoveForward(string nextPosition, map<string, Piece*> Board, int sign)
{
   if(nextPosition[0] == currentPosition_[0] && (nextPosition[1] == currentPosition_[1] + 1*sign) &&
      Board[nextPosition] == nullptr)
   {
        return true;
   } else
        return false;
}

bool Pawn::canCapture(string nextPosition, map<string, Piece*> Board, int sign)
{
    if((nextPosition[1] == (currentPosition_[1] + sign)) &&
       (nextPosition[0] == (currentPosition_[0] - 1) ||  nextPosition[0] == (currentPosition_[0] + 1)) &&
        Board[nextPosition] != nullptr)
    {
         return true;
    } else
         return false;
}

bool Pawn::canEnPassant(string nextPosition, map<string, Piece*> Board, int sign)
{
    /* Left and Right sides are as seen from the piece perspective
    i.e. right side for a black pawn is a left side for a white pawn */
    string leftSideField = string(1,(currentPosition_[0] - sign)) + string(1,currentPosition_[1]);
    string rightSideField = string(1,(currentPosition_[0] + sign)) + string(1,currentPosition_[1]);

    /* En passant is correct if there exists a pawn of the opposite colour to be
    captured and the attacking pawn's next field is not empty */
    if((Board.find(leftSideField) != Board.end()) && Board[leftSideField] != nullptr &&
       (Board[leftSideField] == dynamic_cast<Pawn*>(Board[leftSideField])) &&
       (Board[leftSideField]->getColour() != colour_) &&
      ((Board[leftSideField]->getPreviousPosition())[0] == leftSideField[0]) &&
      ((Board[leftSideField]->getPreviousPosition())[1] == leftSideField[1] + 2*sign) &&
       (nextPosition[0] == currentPosition_[0] - sign) &&
       (nextPosition[1] == currentPosition_[1] + sign))
    {
        return true;
    } else if((Board.find(rightSideField) != Board.end()) && Board[rightSideField] != nullptr &&
              (Board[rightSideField] == dynamic_cast<Pawn*>(Board[rightSideField])) &&
              (Board[rightSideField]->getColour() != colour_) &&
             ((Board[rightSideField]->getPreviousPosition())[0] == rightSideField[0]) &&
             ((Board[rightSideField]->getPreviousPosition())[1] == rightSideField[1] + 2*sign) &&
              (nextPosition[0] == currentPosition_[0] + sign) &&
              (nextPosition[1] == currentPosition_[1] + sign))
    {
        return true;
    }
    return false;
}

bool Pawn::isValidMove(string nextPosition, map<string, Piece*> Board, int& errorCode)
{
    int sign;   // 1 for White pieces, -1 for Black Pieces
    getColour() == Colour::White ? sign = 1 : sign = -1;

    if(canMoveDoubleForward(nextPosition, Board, sign) ||
       canMoveForward(nextPosition, Board, sign) ||
       canCapture(nextPosition, Board, sign) ||
       canEnPassant(nextPosition, Board, sign))
   {
       return true;
   } else
   {
       errorCode = ERROR_INVALID_PAWN_MOVE;
       return false;
   }
}

void Pawn::setFirstMove(bool value)
{
    isFirstMove_ = false;
}
