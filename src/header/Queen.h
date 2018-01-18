#ifndef QUEEN_H
#define QUEEN_H

#include "errors.h"
#include "constants.h"
#include "Piece.h"

class Piece;

class Queen : public Piece
{
    private:

    public:
        Queen(Colour colour, std::string currentPosition);
        virtual ~Queen();

        /* Return true if a Queen's move is valid, false otherwise.
        Queen's move is valid if:
        - Destination is on the same file or rank or diagonal as the source
        - No jumping over other pieces occurs
        The method does not check if fields are valid (within the board), the player
        is right (its their turn), the source file is different to the destination
        file and the destination file is not already occupied by other piece belonging
        to that player.  */
        bool isValidMove(std::string nextPosition, std::map<std::string, Piece*> Board_, int& errorCode);
};

#endif
