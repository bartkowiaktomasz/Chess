#ifndef KING_H
#define KING_H

#include "errors.h"
#include "constants.h"
#include "Piece.h"

class King : public Piece
{
    private:
        bool isFirstMove_;

    public:
        King(Colour colour, std::string currentPosition);
        virtual ~King();

        /* Return true if a Kings's move is valid, false otherwise.
        King's move is valid if:
        - It moves one field in any direction unless the square is already occupied
        - The destination field does not put that king in check
        The method does not check if fields are valid (within the board), the player
        is right (its their turn), the source file is different to the destination
        file and the destination file is not already occupied by other piece belonging
        to that player.  */
        bool isValidMove(std::string nextPosition, std::map<std::string, Piece*> Board_, int& errorCode);

        /* Accessors */
        bool isFirstMove();

        /* Mutators */
        void setFirstMove(bool value);
};

#endif
