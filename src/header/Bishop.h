#ifndef BISHOP_H
#define BISHOP_H

#include "errors.h"
#include "constants.h"
#include "Piece.h"

class Bishop : public Piece
{
    private:

    public:
        Bishop(Colour colour, std::string currentPosition);
        virtual ~Bishop();

        /* Return true if a Bishop's move is valid, false otherwise.
        Bishop's move is valid if:
        - Destination is on the same diagonal as the source
        - No jumping over other pieces occurs
        The method does not check if fields are valid (within the board), the player
        is right (its their turn), the source file is different to the destination
        file and the destination file is not already occupied by other piece belonging
        to that player.  */
        bool isValidMove(std::string nextPosition, std::map<std::string, Piece*> Board_, int& errorCode);
};

#endif
