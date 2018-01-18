#ifndef KNIGHT_H
#define KNIGHT_H

#include "errors.h"
#include "constants.h"
#include "Piece.h"

class Knight : public Piece
{
    private:

    public:
        Knight(Colour colour, std::string currentPosition);
        virtual ~Knight();

        /* Return true if a Knight's move is valid, false otherwise.
        Knight's move is valid if:
        - The destination field is two squares away horizontally and one
        square vertically, or two squares vertically and one square horizontally
        The method does not check if fields are valid (within the board), the player
        is right (its their turn), the source file is different to the destination
        file and the destination file is not already occupied by other piece belonging
        to that player.  */
        bool isValidMove(std::string nextPosition, std::map<std::string, Piece*> Board_, int& errorCode);
};

#endif
