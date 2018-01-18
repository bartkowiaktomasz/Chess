#ifndef PAWN_H
#define PAWN_H

#include "errors.h"
#include "constants.h"
#include "Piece.h"

class Pawn : public Piece
{
    private:
        bool isFirstMove_;

        /* Returns true if a pawn can make a step two positions forwards (it can
        only be done if it their first move), false otherwise. */
        bool canMoveDoubleForward(std::string nextPosition, std::map<std::string, Piece*> Board, int sign);

        /* Returns true if a pawn can make a single step forwards, false otherwise. */
        bool canMoveForward(std::string nextPosition, std::map<std::string, Piece*> Board, int sign);

        /* Returns true if a pawn can capture a piece, false otherwise. */
        bool canCapture(std::string nextPosition, std::map<std::string, Piece*> Board, int sign);

        /* Returns true if a pawn can capture a piece, false otherwise. */
        bool canEnPassant(std::string nextPosition, std::map<std::string, Piece*> Board, int sign);

        /* Returns true if a pawn can be promoted, false otherwise. */
        void checkForPromotion(std::string nextPosition, std::map<std::string, Piece*> Board);

    public:
        Pawn(Colour colour, std::string currentPosition);
        virtual ~Pawn();

        /* Return true if a Pawn's move is valid, false otherwise.
        Pawn's move is valid if:
        - It moves one step forward (no piece can block that position)
        - It moves two steps forward (only possible if the positions are not
        blocked and the pawn has not moved before)
        - It captures the opposite piece (normal capture or En passant)
        The method does not check if fields are valid (within the board), the player
        is right (its their turn), the source file is different to the destination
        file and the destination file is not already occupied by other piece belonging
        to that player.  */
        bool isValidMove(std::string nextPosition, std::map<std::string, Piece*> Board_, int& errorCode);

        /* Mutators */
        void setFirstMove(bool value);
};

#endif
