#ifndef PIECE_H
#define PIECE_H

#include "errors.h"
#include "constants.h"

#include <string>
#include <map>

enum class Colour {White, Black};

class Piece
{
    friend class ChessBoard;

    protected:
        Colour colour_;
        std::string currentPosition_;
        std::string previousPosition_;
        std::string symbol_;

        /* Given two positions return true if they are in the same
        rank/file/diagonal and false otherwise. */
        bool isSameRank(std::string firstPosition, std::string secondPosition);
        bool isSameFile(std::string firstPosition, std::string secondPosition);
        bool isSameDiagonal(std::string firstPosition, std::string secondPosition);

        /* Given two positions and a chessboard, return true if there are no pieces
        between the positions and false otherwise. The methods also check if the
        positions are in the same rank/file/diagonal. */
        bool isClearRank(std::string firstPosition,
            std::string secondPosition, std::map<std::string, Piece*> Board_);
        bool isClearFile(std::string firstPosition,
            std::string secondPosition, std::map<std::string, Piece*> Board_);
        bool isClearDiagonal(std::string firstPosition,
            std::string secondPosition, std::map<std::string, Piece*> Board_);

        /* Mutators */
        void setCurrentPosition(std::string currentPosition);
        void setPreviousPosition(std::string currentPosition);

    public:
        Piece(Colour colour, std::string currentPosition);
        virtual ~Piece();

        /* Abstract method returning true if a move for a particular piece
        is valid, false otherwise. */
        virtual bool isValidMove(std::string nextPosition, std::map<std::string, Piece*> Board_, int& errorCode) = 0;

        /* Accessors */
        Colour getColour();
        std::string getCurrentPosition();
        std::string getPreviousPosition();
};

#endif
