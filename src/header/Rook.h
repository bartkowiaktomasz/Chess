#ifndef ROOK_H
#define ROOK_H

#include "errors.h"
#include "constants.h"
#include "Piece.h"

class Piece;

class Rook : public Piece
{
    private:
        bool isFirstMove_;

    public:
    Rook(Colour colour, std::string currentPosition);
    virtual ~Rook();

    /* Return true if a Rook's move is valid, false otherwise.
    Rook's move is valid if:
    - Destination is on the same file or rank as the source
    - No jumping over other pieces occurs
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
