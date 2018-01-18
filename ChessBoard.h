#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include "src/header/errors.h"
#include "src/header/constants.h"
#include "src/header/Piece.h"
#include "src/header/Rook.h"
#include "src/header/Knight.h"
#include "src/header/Bishop.h"
#include "src/header/Queen.h"
#include "src/header/King.h"
#include "src/header/Pawn.h"

#include <string>
#include <vector>
#include <map>

enum class Winner {White, Black, Draw};

class ChessBoard
{
    private:
        std::map<std::string, Piece*> Board_;
        Winner gameWinner_;
        std::string whiteKingPosition_;
        std::string blackKingPosition_;
        bool whiteTurn_;
        bool isGameOn_;

        /* True if a board visualisation should be printed to the terminal,
        false otherwise. Default setting: false. */
        bool printBoard_;

        /* Initializes a checkboard (pieces put on their initial positions) and
        gives an informative message of the game rules to the player. */
        void initialize();

        /* Cleans the checkboard, deallocates memory. */
        void clean();

        /* Changes the state of "whiteTurn_" member to opposite. */
        void switchTurn();

        /* Given a position on a chessboard, return the vector of strings
        (positions) of all own/opposite pieces that can move (the move is valid)
        to that position. */
        std::vector<std::string> findMyPiecesThatCanMoveToPosition(std::string position);
        std::vector<std::string> findMyPiecesOtherThanKingThatCanMoveToPosition(std::string position);
        std::vector<std::string> findOppositePiecesThatCanMoveToPosition(std::string position);

        /* If pawn promotion conditions are met, prompt user for a piece type
        that they want to promote their pawn to, and instantiate that object on
        the board (previously deleting the pawn object). */
        void pawnPromotion(std::string currentPosition, std::string pawnPosition, int& errorCode);

        /* If en-passant capture conditions are met, deallocate captured piece. */
        void enPassant(std::string currentPosition, std::string nextPosition, int& errorCode);

        /* The following methods take three input arguments:
        - current (source) position string
        - destination position string
        - error code (passed by reference, the code is to be changed by the
        method and then returned in the submitMove() method with appropriate
        informative message).
        All of the methods are checking if given move is valid. */
        bool isRightPlayer(std::string currentPosition, int& errorCode);
        bool isDestinationEqualSource(std::string currentPosition, std::string nextPosition, int& errorCode);
        bool isDestinationOccupiedBySameColour(std::string currentPosition, std::string nextPosition, int& errorCode);
        bool isValidMove(std::string currentPosition, std::string nextPosition, int& errorCode);
        bool fieldsValid(std::string currentPosition, std::string nextPosition, int& errorCode);
        bool isCastling(std::string text, std::string rookPosition, int& errorCode);

        /* Methods returning a particular game state information. */
        bool isGameOn();
        /* Stalemate is a sitaution at which no piece can make any valid move
        but their own king is not in check */
        bool isStaleMate();
        bool isCheck(std::string kingPosition);
        bool isMyselfInCheck(std::string kingPosition);
        bool isCheckMate(std::string kingPosition, std::string attackingPiece);

        /* Methods printing informative messages to the player. */
        void printBoard();
        void printError(std::string currentPosition, std::string nextPosition, int errorCode);
        void printGameResult();
        void printMove(std::string currentPosition, std::string nextPosition);
        void printCapture(std::string nextPosition);

        /* Given position string return a piece type name, i.e. "Pawn". */
        std::string pieceType(std::string currentPosition);

        /* Accessors */
        Colour getPlayerTurn();
        std::string getMyKingPosition();

    public:
        ChessBoard();
        ~ChessBoard();

        /* Interface methods exposed to the client. sumbitMove() allows the player
        to move his piece on the board, and the resetBoard() is used to bring
        the chessboard to the initial state. */
        void submitMove(std::string currentPosition, std::string nextPosition);
        void resetBoard();
};

#endif
