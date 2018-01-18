#include "header/Piece.h"

#include <iostream>
#include <string>
#include <cmath>

using namespace std;

Piece::Piece(Colour colour, string currentPosition):
    colour_(colour), currentPosition_(currentPosition)  {}

Piece::~Piece() {}

bool Piece::isSameRank(string firstPosition, string secondPosition)
{
    return (firstPosition[1] == secondPosition[1] ? true : false);
}

bool Piece::isSameFile(string firstPosition, string secondPosition)
{
    return (firstPosition[0] == secondPosition[0] ? true : false);
}

bool Piece::isSameDiagonal(string firstPosition, string secondPosition)
{
    return (abs(secondPosition[0] - firstPosition[0]) ==
            abs(secondPosition[1] - firstPosition[1]) ? true : false);
}

bool Piece::isClearRank(string firstPosition, string secondPosition, map<string, Piece*> Board)
{
    if(!isSameRank(firstPosition, secondPosition))
        return false;
    string position = secondPosition;
    int xIncrement;

    if(firstPosition[0] > secondPosition[0])
        xIncrement = 1;
    else
        xIncrement = -1;

    while((position[0] += xIncrement) != firstPosition[0])
    {
        if(Board[position] != nullptr )
            return false;
    }
    return true;
}

bool Piece::isClearFile(string firstPosition, string secondPosition, map<string, Piece*> Board)
{
    if(!isSameFile(firstPosition, secondPosition))
        return false;
    string position = secondPosition;
    int yIncrement;
    if(firstPosition[1] > secondPosition[1])
        yIncrement = 1;
    else
        yIncrement = -1;

    while((position[1] += yIncrement) != firstPosition[1])
    {
        if(Board[position] != nullptr )
            return false;
    }
    return true;
}

bool Piece::isClearDiagonal(string firstPosition, string secondPosition, map<string, Piece*> Board)
{
    if(!isSameDiagonal(firstPosition, secondPosition))
        return false;
    string position = secondPosition;
    int xIncrement, yIncrement;
    if(firstPosition[0] > secondPosition[0] && firstPosition[1] > secondPosition[1])
    {
        xIncrement = 1;
        yIncrement = 1;
    }
    else if(firstPosition[0] > secondPosition[0] && firstPosition[1] < secondPosition[1])
    {
        xIncrement = 1;
        yIncrement = -1;
    }
    else if(firstPosition[0] < secondPosition[0] && firstPosition[1] > secondPosition[1])
    {
        xIncrement = -1;
        yIncrement = 1;
    }
    else
    {
        xIncrement = -1;
        yIncrement = -1;
    }

    while((position[0] += xIncrement) != firstPosition[0])
    {
        position[1] += yIncrement;
        if(Board[position] != nullptr )
            return false;
    }
    return true;
}

void Piece::setCurrentPosition(string currentPosition)
{
    currentPosition_ = currentPosition;
}

void Piece::setPreviousPosition(string previousPosition)
{
    previousPosition_ = previousPosition;
}

Colour Piece::getColour()
{
    return colour_;
}

string Piece::getCurrentPosition()
{
    return currentPosition_;
}

string Piece::getPreviousPosition()
{
    return previousPosition_;
}
