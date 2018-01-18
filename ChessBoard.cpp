#include "ChessBoard.h"

#include <iostream>
#include <algorithm>

using namespace std;

ChessBoard::ChessBoard()
{
    initialize();
}

ChessBoard::~ChessBoard()
{
    clean();
}

void ChessBoard::submitMove(string currentPosition, string nextPosition)
{
    int errorCode = SUCCESS;
    /* Change the first letter to uppercase */
    currentPosition[0] = toupper(currentPosition[0]);
    nextPosition[0] = toupper(nextPosition[0]);

    if(isGameOn_)
    {
        if(isCastling(currentPosition, nextPosition, errorCode))
        {
            if(errorCode == ERROR_PUTTING_OWN_KING_IN_CHECK ||
               errorCode == ERROR_CASTLING_IMPOSSIBLE)
            {
                printError(currentPosition, nextPosition, errorCode);
                return;
            }
            else
            {
                printBoard();
                switchTurn();
                return;
            }
        }
        else if(isValidMove(currentPosition, nextPosition, errorCode))
        {
            printMove(currentPosition, nextPosition);
            /* If King is being moved, change its position on the board */
            if(!(currentPosition.compare(whiteKingPosition_)))
                whiteKingPosition_ = nextPosition;
            else if(!(currentPosition.compare(blackKingPosition_)))
                blackKingPosition_ = nextPosition;

            printCapture(nextPosition);

            /* If the piece is a Pawn */
            if(dynamic_cast<Pawn*>(Board_[currentPosition]))
            {
                /* Check for Pawn promotion */
                pawnPromotion(currentPosition, nextPosition, errorCode);
                /* Check for En Passant */
                enPassant(currentPosition, nextPosition, errorCode);
                if(errorCode == ERROR_PUTTING_OWN_KING_IN_CHECK)
                {
                    printError(currentPosition, nextPosition, errorCode);
                    return;
                }
            }

            /* If the piece is either Pawn or Rook or King, set their firstMove
            member to false */
            if(dynamic_cast<Pawn*>(Board_[currentPosition]))
                dynamic_cast<Pawn*>(Board_[currentPosition])->setFirstMove(false);
            if(dynamic_cast<Rook*>(Board_[currentPosition]))
                dynamic_cast<Rook*>(Board_[currentPosition])->setFirstMove(false);
            if(dynamic_cast<King*>(Board_[currentPosition]))
                dynamic_cast<King*>(Board_[currentPosition])->setFirstMove(false);

            /* "capturedPiece" will store captured piece if there is need to set
            back to previous settings */
            Piece* capturedPiece = nullptr;
            if(Board_[nextPosition] != nullptr)
            {
                capturedPiece = Board_[nextPosition];
                Board_[nextPosition] = nullptr;
            }

            /* Set piece position */
            string previousPosition = Board_[currentPosition]->getPreviousPosition();
            Board_[currentPosition]->setPreviousPosition(currentPosition);
            Board_[currentPosition]->setCurrentPosition(nextPosition);
            Board_[nextPosition] = nullptr;
            Board_[nextPosition] = Board_[currentPosition];
            Board_[currentPosition] = nullptr;

            /* Check for check */
            /* Set kingPosition to the king position of the opposite colour */
            string oppositeKingPosition;
            string kingPosition = getMyKingPosition();
            if(getPlayerTurn() == Colour::White)
                oppositeKingPosition = blackKingPosition_;
            else
                oppositeKingPosition = whiteKingPosition_;

            /* If it is King who is moving, change its transient position */
            if(dynamic_cast<King*>(Board_[nextPosition]))
                kingPosition = nextPosition;

            /* Check if the move exposed their own king to check.
            If yes, set back to previous settings and return error. */
            if(isMyselfInCheck(kingPosition))
            {
                Board_[currentPosition] = Board_[nextPosition];
                Board_[nextPosition] = capturedPiece;
                errorCode = ERROR_PUTTING_OWN_KING_IN_CHECK;
                printError(currentPosition, nextPosition, errorCode);
                return;
            }
            delete capturedPiece;

            printBoard();
            if(isCheck(oppositeKingPosition))
            {
                vector<string> threateningPieces;
                string attackingPiece;

                threateningPieces = findMyPiecesThatCanMoveToPosition(oppositeKingPosition);
                cout << "CHECK! King at " + oppositeKingPosition + " is attacked by the piece at " + threateningPieces[0] << endl;
                attackingPiece = threateningPieces[0];

                /* Check for check mate */
                if(isCheckMate(oppositeKingPosition, attackingPiece))
                {
                    cout << endl << "CHECK MATE! King at position " + oppositeKingPosition +
                    " is at checkmate" << endl;

                    /* Set winner */
                    if(Board_[attackingPiece]->getColour() == Colour::White)
                        gameWinner_ = Winner::White;
                    else
                        gameWinner_ = Winner::Black;

                    isGameOn_ = false;
                    printGameResult();
                    exit(SUCCESS);
                }
            }
            switchTurn();

            /* Check for Stale mate */
            if(isStaleMate())
            {
                gameWinner_ = Winner::Draw;
                isGameOn_ = false;
                printGameResult();
                exit(SUCCESS);
            }

        } else
        {
            printError(currentPosition, nextPosition, errorCode);
        }
    } else
    {
        errorCode = ERROR_GAME_IS_OFF;
        printError(currentPosition, nextPosition, errorCode);
    }
}

void ChessBoard::resetBoard()
{
    clean();
    initialize();
}

void ChessBoard::initialize()
{
    gameWinner_ = Winner::Draw;
    whiteTurn_ = true;
    isGameOn_ = true;
    printBoard_ = true;

    string position;

    Board_["A1"] = new Rook(Colour::White, "A1");
    Board_["B1"] = new Knight(Colour::White, "B1");
    Board_["C1"] = new Bishop(Colour::White, "C1");
    Board_["D1"] = new Queen(Colour::White, "D1");
    Board_["E1"] = new King(Colour::White, "E1");
    Board_["F1"] = new Bishop(Colour::White, "F1");
    Board_["G1"] = new Knight(Colour::White, "G1");
    Board_["H1"] = new Rook(Colour::White, "H1");
    for(position = "A2"; position[0] <= HIGHEST_FILE; position[0]++)
        Board_[position] = new Pawn(Colour::White, position);

    Board_["A8"] = new Rook(Colour::Black, "A8");
    Board_["B8"] = new Knight(Colour::Black, "B8");
    Board_["C8"] = new Bishop(Colour::Black, "C8");
    Board_["D8"] = new Queen(Colour::Black, "D8");
    Board_["E8"] = new King(Colour::Black, "E8");
    Board_["F8"] = new Bishop(Colour::Black, "F8");
    Board_["G8"] = new Knight(Colour::Black, "G8");
    Board_["H8"] = new Rook(Colour::Black, "H8");
    for(position = "A7"; position[0] <= HIGHEST_FILE; position[0]++)
        Board_[position] = new Pawn(Colour::Black, position);

    for(int rank = 3; rank <= 6; rank++)
    {
        for(char file = LOWEST_FILE; file <= HIGHEST_FILE; file++)
            Board_[string(1,file) + to_string(rank)] = nullptr;
    }
    whiteKingPosition_ = "E1";
    blackKingPosition_ = "E8";

    cout << "===========================" << endl;
    cout << "New chessboard created" << endl;
    cout << "===========================" << endl;
    cout << "Rules: \n- To move a piece, provide two strings: source and"
    << " destination field, i.e \"A2\", \"A3\","
    << endl << "- To do castling, type \"Castling\" and a Rook's source field,"
    << endl << "- If a pawn gets promoted, please provide a piece type you want"
    << " the pawn to be promoted to." << endl << endl;
    printBoard();
    cout << "First turn belongs to White!" << endl << endl;
}

void ChessBoard::clean()
{
    map<string, Piece*>::iterator it;
    for(it = Board_.begin(); it != Board_.end(); ++it)
        delete it->second;
}

void ChessBoard::switchTurn()
{
    whiteTurn_ = !whiteTurn_;
}

vector<string> ChessBoard::findMyPiecesThatCanMoveToPosition(string position)
{
    int errorCode;
    vector<string> pieces;
    map<string, Piece*>::iterator it;
    /* Iterate through the board and for each of the pieces of current player
    check if their move to the destination field is valid */
    for(it = Board_.begin(); it != Board_.end(); ++it)
    {
        if((it->second) != nullptr &&
           (it->second)->getColour() == getPlayerTurn() &&
            isValidMove((it->first), position, errorCode))
        {
            pieces.push_back((it->first));
        }
    }
    return pieces;
}

vector<string> ChessBoard::findMyPiecesOtherThanKingThatCanMoveToPosition(string position)
{
    int errorCode;
    vector<string> pieces;
    map<string, Piece*>::iterator it;
    /* Iterate through the board and for each of the pieces of current player
    check if their move to the destination field is valid */
    for(it = Board_.begin(); it != Board_.end(); ++it)
    {
        if((it->second) != nullptr &&
           (it->second)->getColour() == getPlayerTurn() &&
            isValidMove((it->first), position, errorCode) &&
            !dynamic_cast<King*>(it->second))
        {
            pieces.push_back((it->first));
        }
    }
    return pieces;
}

vector<string> ChessBoard::findOppositePiecesThatCanMoveToPosition(string position)
{
    vector<string> pieces;
    switchTurn();
    pieces = findMyPiecesThatCanMoveToPosition(position);
    switchTurn();
    return pieces;
}

void ChessBoard::pawnPromotion(string currentPosition, string nextPosition, int& errorCode)
{
    if((whiteTurn_ && nextPosition[1] == HIGHEST_RANK) ||
    (!whiteTurn_ && nextPosition[1] == LOWEST_RANK))
    {
        string pieceType;
        Colour colour;
        (whiteTurn_ == true) ? colour = Colour::White : colour = Colour::Black;
        Piece* piece = Board_[currentPosition];
        Board_[currentPosition] = nullptr;

        /* Check if the move exposed their own king to check.
        If yes, set back to previous settings and return error. */
        if(isMyselfInCheck(getMyKingPosition()))
        {
            Board_[currentPosition] = piece;
            errorCode = ERROR_PUTTING_OWN_KING_IN_CHECK;
            return;
        }

        cout << endl << "Pawn at position " << nextPosition << " has been promoted" << endl;
        cout << "Which piece type do you want your pawn to be promoted to?"
        << " (type: \"Queen\", \"Rook\", \"Bishop\", \"Knight\", \"Pawn\"): \n";
        cin >> pieceType;
        cout << endl;
        while(true)
        {
            if(!pieceType.compare("Queen"))
            {
                Board_[currentPosition] = new Queen(colour, currentPosition);
                break;
            }
            else if(!pieceType.compare("Rook"))
            {
                Board_[currentPosition] = new Rook(colour, currentPosition);
                break;
            }
            else if(!pieceType.compare("Bishop"))
            {
                Board_[currentPosition] = new Bishop(colour, currentPosition);
                break;
            }
            else if(!pieceType.compare("Knight"))
            {
                Board_[currentPosition] = new Knight(colour, currentPosition);
                break;
            }
            else if(!pieceType.compare("Pawn"))
            {
                Board_[currentPosition] = new Pawn(colour, currentPosition);
                break;
            }
            else
            {
                cerr << "Wrong piece type. Please try again:" << endl;;
                cin >> pieceType;
            }
        }
        delete piece;
        cout << "The pawn has become a " << pieceType;
    }
}

void ChessBoard::enPassant(string currentPosition, string nextPosition, int& errorCode)
{
    /* "capturedPiece" will store captured piece if there is need to set
    back to previous settings */
    Piece* capturedPiece = nullptr;
    int sign;

    /* "position" is the position of captured piece. Initialize its value
    to "nextPosition" and then add move one position up/down */
    string position = nextPosition;
    whiteTurn_ == true ? sign = 1 : sign = -1;
    position[1] -= sign;

    if((abs(nextPosition[0] - currentPosition[0]) == 1) &&
    (abs(nextPosition[1] - currentPosition[1]) == 1) &&
    Board_[nextPosition] == nullptr &&
    Board_[position] != nullptr)
    {
        capturedPiece = Board_[position];
        Board_[position] = nullptr;

        /* Check if en passant resulted in putting own king in check.
        If yes, set back to previous settings and return error. */
        if(isMyselfInCheck(getMyKingPosition()))
        {
            Board_[position] = capturedPiece;
            errorCode = ERROR_PUTTING_OWN_KING_IN_CHECK;
            return;
        }
        delete capturedPiece;
        cout << endl << "Piece at position " + position + " captured en-passant";
    }
}

bool ChessBoard::isRightPlayer(string currentPosition, int& errorCode)
{
    if(Board_[currentPosition] != nullptr)
    {
        if(Board_[currentPosition]->getColour() == getPlayerTurn())
        return true;
        else
        {
            errorCode = ERROR_WRONG_PLAYER;
            return false;
        }
    } else
    {
        errorCode = ERROR_SOURCE_FIELD_EMPTY;
        return false;
    }
}

bool ChessBoard::isDestinationEqualSource(string currentPosition, string nextPosition, int& errorCode)
{
    if(!currentPosition.compare(nextPosition))
    {
        errorCode = ERROR_SOURCE_SAME_AS_DESTINATION;
        return true;
    } else
    return false;
}

bool ChessBoard::isDestinationOccupiedBySameColour(string currentPosition, string nextPosition, int& errorCode)
{
    if(Board_[nextPosition] != nullptr &&
       Board_[nextPosition]->getColour() == getPlayerTurn())
    {
        errorCode = ERROR_OCCUPIED_BY_SAME_COLOUR;
        return true;
    } else
        return false;
}

bool ChessBoard::isValidMove(string currentPosition, string nextPosition, int& errorCode)
{
    if(isRightPlayer(currentPosition, errorCode) &&
    fieldsValid(currentPosition, nextPosition, errorCode) &&
    !isDestinationEqualSource(currentPosition, nextPosition, errorCode) &&
    !isDestinationOccupiedBySameColour(currentPosition, nextPosition, errorCode) &&
    Board_[currentPosition]->isValidMove(nextPosition, Board_, errorCode))
    {
        return true;
    } else
    return false;
}

bool ChessBoard::fieldsValid(string currentPosition, string nextPosition, int& errorCode)
{
    char current_file = currentPosition[0];
    char next_file = nextPosition[0];

    /* Change positions' substrings (without the first letters) to integers */
    int current_rank = stoi(currentPosition.substr(1, currentPosition.length()));
    int next_rank = stoi(nextPosition.substr(1, nextPosition.length()));

    if(current_file >= LOWEST_FILE && current_file <= HIGHEST_FILE &&
       current_rank >= 1 && current_rank <= BOARD_SIZE)
    {
        if(next_file >= LOWEST_FILE && next_file <= HIGHEST_FILE &&
           next_rank >= 1 && next_rank <= BOARD_SIZE)
        {
            return true;
        } else
        {
            errorCode = ERROR_INVALID_SOURCE_FIELD;
            return false;
        }
    } else
    {
        errorCode = ERROR_INVALID_DESTINATION_FIELD;
        return false;
    }
}

bool ChessBoard::isCastling(string text, string rookPosition, int& errorCode)
{
    /* Convert input text to lower letters */
    transform(text.begin(), text.end(), text.begin(), ::tolower);
    if(!text.compare("castling") &&
      (!rookPosition.compare(WHITE_ROOK_INITIAL_POSITION) ||
       !rookPosition.compare(BLACK_ROOK_INITIAL_POSITION)))
      {
          int increment;
          string kingPosition = getMyKingPosition();
          vector<string> threateningPieces;

          (kingPosition[0] > rookPosition[0]) ? increment = -1 : increment = 1;

          string transientKingPosition = kingPosition; transientKingPosition[0] += increment;
          string newKingPosition = transientKingPosition; newKingPosition[0] += increment;

          threateningPieces = findOppositePiecesThatCanMoveToPosition(transientKingPosition);
          if((dynamic_cast<King*>(Board_[kingPosition]))->isFirstMove() &&
             (dynamic_cast<Rook*>(Board_[rookPosition]))->isFirstMove() &&
             Board_[kingPosition]->isClearRank(kingPosition, rookPosition, Board_) &&
             threateningPieces.empty())
          {

              Board_[kingPosition]->setPreviousPosition(kingPosition);
              Board_[kingPosition]->setCurrentPosition(newKingPosition);
              Board_[newKingPosition] = Board_[kingPosition];
              Board_[kingPosition] = nullptr;

              Board_[rookPosition]->setPreviousPosition(rookPosition);
              Board_[rookPosition]->setCurrentPosition(transientKingPosition);
              Board_[transientKingPosition] = Board_[rookPosition];
              Board_[rookPosition] = nullptr;

              /* Check if castling resulted in putting own king in check.
              If yes, set back to previous settings and return error. */
              if(isMyselfInCheck(newKingPosition))
              {
                  Board_[kingPosition] = Board_[newKingPosition];
                  Board_[newKingPosition] = nullptr;
                  Board_[kingPosition]->setPreviousPosition(kingPosition);
                  Board_[kingPosition]->setCurrentPosition(kingPosition);

                  Board_[rookPosition] = Board_[transientKingPosition];
                  Board_[transientKingPosition] = nullptr;
                  Board_[rookPosition]->setPreviousPosition(rookPosition);
                  Board_[rookPosition]->setCurrentPosition(rookPosition);

                  errorCode = ERROR_PUTTING_OWN_KING_IN_CHECK;
                  return true;
              }
              cout << "Castling with Rook at position " + rookPosition + "." << endl;
              return true;
          } else
          {
              errorCode = ERROR_CASTLING_IMPOSSIBLE;
              return true;
          }
      }
      return false;
}

bool ChessBoard::isGameOn()
{
    return isGameOn_;
}

bool ChessBoard::isStaleMate()
{
    map<string, Piece*>::iterator it;
    string position;
    int UNUSED_ERROR_CODE;
    Piece* capturedPiece = nullptr;
    for(int rank = 1; rank <= BOARD_SIZE; rank++)
    {
        for(char file = LOWEST_FILE; file <= HIGHEST_FILE; file++)
        {
            position = string(1,file) + to_string(rank);
            for(it = Board_.begin(); it != Board_.end(); ++it)
            {
                if((it->second) != nullptr &&
                   (it->second)->getColour() == getPlayerTurn() &&
                    isValidMove((it->first), position, UNUSED_ERROR_CODE))
                {

                    if(Board_[position] != nullptr)
                    {
                        capturedPiece = Board_[position];
                        Board_[position] = nullptr;
                    }

                    string previousPosition = Board_[(it->first)]->getPreviousPosition();
                    string kingPosition = getMyKingPosition();
                    Board_[(it->first)]->setPreviousPosition((it->first));
                    Board_[(it->first)]->setCurrentPosition(position);
                    Board_[position] = Board_[(it->first)];
                    Board_[(it->first)] = nullptr;

                    /* If it is King who is moving, change its transient position */
                    if(dynamic_cast<King*>(Board_[position]))
                        kingPosition = position;

                    if(!isMyselfInCheck(kingPosition))
                    {
                        Board_[(it->first)] = Board_[position];
                        Board_[position] = capturedPiece;
                        Board_[(it->first)]->setPreviousPosition(previousPosition);
                        Board_[(it->first)]->setCurrentPosition((it->first));
                        return false;
                    }
                    Board_[(it->first)] = Board_[position];
                    Board_[position] = capturedPiece;
                    Board_[(it->first)]->setPreviousPosition(previousPosition);
                    Board_[(it->first)]->setCurrentPosition((it->first));
                }
            }
        }
    }
    return true;
}

bool ChessBoard::isCheck(string kingPosition)
{
    vector<string> threateningPieces = findMyPiecesThatCanMoveToPosition(kingPosition);
    if(!threateningPieces.empty())
        return true;
    else
        return false;
}

bool ChessBoard::isMyselfInCheck(string kingPosition)
{
    switchTurn();
    if(isCheck(kingPosition))
    {
        switchTurn();
        return true;
    } else
    {
        switchTurn();
        return false;
    }

}

bool ChessBoard::isCheckMate(string kingPosition, string attackingPiece)
{
    int UNUSED_ERROR_CODE;
    string destination;
    vector<string> possibleKingDestinations;
    vector<string> threatenedDestinations;

    /* Switch turn to allow for checking movement possibilities
    for the opposite King (under attack) */
    switchTurn();

    for(int rank = 1; rank <= BOARD_SIZE; rank++)
    {
        for(char file = LOWEST_FILE; file <= HIGHEST_FILE; file++)
        {
            destination = string(1,file) + to_string(rank);
            if(isValidMove(kingPosition, destination, UNUSED_ERROR_CODE))
                possibleKingDestinations.push_back(destination);
        }
    }

    /* Switch turn back to attacker to check if attacking piecies
    are able to threaten the king at his every possible destination */
    switchTurn();
    vector<std::string> threateningPieces;
    unsigned int numberOfThreatenedDestinations = 0;
    for(unsigned int i = 0; i < possibleKingDestinations.size(); i++)
    {
        threateningPieces = findMyPiecesThatCanMoveToPosition(possibleKingDestinations[i]);
        if(!threateningPieces.empty())
        {
            threatenedDestinations.push_back(possibleKingDestinations[i]);
            numberOfThreatenedDestinations++;
        }
    }

    /* Switch to defender and check if they can destroy the attacking piece */
    switchTurn();
    vector<string> defenderPieces = findMyPiecesThatCanMoveToPosition(attackingPiece);
    if(!defenderPieces.empty())
    {
        /* Back to attacker */
        switchTurn();
        return false;
    }

    /* Check if the defender can block the attacking piece */
    vector<string> possiblePositionsToBlock;
    string position = attackingPiece;
    int xIncrement, yIncrement;
    if(Board_[attackingPiece]->isClearRank(attackingPiece, kingPosition, Board_))
        {
            if(kingPosition[0] - attackingPiece[0] > 0)
                xIncrement = 1;
            else
                xIncrement = -1;

            /* String compare returns zero if strings are equal! */
            while(position.compare(kingPosition))
            {
                possiblePositionsToBlock = findMyPiecesOtherThanKingThatCanMoveToPosition(position);
                if(!possiblePositionsToBlock.empty())
                {
                    /* Switch back to attacker */
                    switchTurn();
                    return false;
                }
                position[0] += xIncrement;
            }
            switchTurn();
            return true;
        }
    if(Board_[attackingPiece]->isClearFile(attackingPiece, kingPosition, Board_))
        {
            if(kingPosition[1] - attackingPiece[1] > 0)
                yIncrement = 1;
            else
                yIncrement = -1;

            // String compare returns zero if strings are equal!
            while(position.compare(kingPosition))
            {
                possiblePositionsToBlock = findMyPiecesOtherThanKingThatCanMoveToPosition(position);
                if(!possiblePositionsToBlock.empty())
                {
                    /* Switch back to attacker */
                    switchTurn();
                    return false;
                }
                position[1] += yIncrement;
            }
            switchTurn();
            return true;
        }
    if(Board_[attackingPiece]->isClearDiagonal(attackingPiece, kingPosition, Board_))
        {
            if(kingPosition[0] > attackingPiece[0] &&
               kingPosition[1] > attackingPiece[1])
            {
                xIncrement = 1;
                yIncrement = 1;
            }
            else if(kingPosition[0] > attackingPiece[0] &&
                    kingPosition[1] < attackingPiece[1])
            {
                xIncrement = 1;
                yIncrement = -1;
            }
            else if(kingPosition[0] < attackingPiece[0] &&
                    kingPosition[1] > attackingPiece[1])
            {
                xIncrement = -1;
                yIncrement = 1;
            }
            else
            {
                xIncrement = -1;
                yIncrement = -1;
            }

            while(!position.compare(kingPosition))
            {
                possiblePositionsToBlock = findMyPiecesOtherThanKingThatCanMoveToPosition(position);
                if(!possiblePositionsToBlock.empty())
                {
                    /* Switch back to attacker */
                    switchTurn();
                    return false;
                }
                position[0] += xIncrement;
                position[1] += yIncrement;
            }
            switchTurn();
            return true;
        }

    /* Switch back to attacker */
    switchTurn();

    /* If the number of possible destinations of the king under attack is equal
    to the number of threatened destination (each possible destination is under
    attack), we have a Check Mate. If the flow reached that point, it means that
    the attacking piece could not be destroyed nor blocked */
    if(numberOfThreatenedDestinations > 0 &&
       possibleKingDestinations.size() == numberOfThreatenedDestinations)
    {
        return true;
    }
    return false;
}

void ChessBoard::printBoard()
{
    if(printBoard_)
    {
        cout << endl;
        cout << "    A B C D E F G H" << endl << endl;
        for(int rank = BOARD_SIZE; rank >= 1; --rank)
        {
            cout << to_string(rank) + "   ";
            for(char file = 'A'; file <= 'H'; file++)
            {
                if(Board_[string(1,file) + to_string(rank)] == nullptr)
                cout << "- ";
                else
                cout << Board_[string(1,file) + to_string(rank)]->symbol_ + " ";
            }
            cout << endl;
        }
        cout << endl;
    }
}

void ChessBoard::printError(string currentPosition, string nextPosition, int errorCode)
{
    switch (errorCode)
    {
        case ERROR_INVALID_SOURCE_FIELD:
            cerr << endl << "Error: " + currentPosition + " is an Invalid source field. "
            "Allowed files are A-H and allowed ranks are 1-8." << endl;
            break;

        case ERROR_INVALID_DESTINATION_FIELD:
            cerr << endl << "Error: " + nextPosition + " is an Invalid destination field. "
            "Allowed files are A-H and allowed ranks are 1-8." << endl;
            break;

        case ERROR_WRONG_PLAYER:
            cerr << endl << "Error: Wrong player." << endl;
            break;

        case ERROR_SOURCE_FIELD_EMPTY:
            cerr << endl << "Error: No piece on the source field." << endl;
            break;

        case ERROR_SOURCE_SAME_AS_DESTINATION:
            cerr << endl << "Error: Source field the same as destination." << endl;
            break;

        case ERROR_OCCUPIED_BY_SAME_COLOUR:
            cerr << endl << "Error: Destination field is already occupied by a piece of the"
            " same colour." << endl;
            break;

        case ERROR_INVALID_ROOK_MOVE:
            cerr << endl << "Error: Invalid Rook move." << endl;
            break;

        case ERROR_INVALID_QUEEN_MOVE:
            cerr << endl << "Error: Invalid Queen move." << endl;
            break;

        case ERROR_INVALID_BISHOP_MOVE:
            cerr << endl << "Error: Invalid Bishop move." << endl;
            break;

        case ERROR_INVALID_KNIGHT_MOVE:
            cerr << endl << "Error: Invalid Knight move." << endl;
            break;

        case ERROR_INVALID_KING_MOVE:
            cerr << endl << "Error: Invalid King move." << endl;
            break;

        case ERROR_INVALID_PAWN_MOVE:
            cerr << endl << "Error: Invalid Pawn move." << endl;
            break;

        case ERROR_GAME_IS_OFF:
            cerr << endl << "Error: Game is off." << endl;
            break;

        case ERROR_CASTLING_IMPOSSIBLE:
            cerr << endl << "Error: Castling impossible." << endl;
            break;

        case ERROR_PUTTING_OWN_KING_IN_CHECK:
            cerr << endl << "Error: Piece cannot expose their king to check." << endl;
            break;
    }
}

void ChessBoard::printGameResult()
{
    if(gameWinner_ == Winner::White)
    {
        cout << endl << endl;
        for(int i = 0; i < 3; i++)
            cout << "------------- WHITE TEAM WINS! -------------" << endl;
        cout << endl << endl;
    }
    else if(gameWinner_ == Winner::Black)
    {
        cout << endl << endl;
        for(int i = 0; i < 3; i++)
            cout << "------------- BLACK TEAM WINS! -------------" << endl;
        cout << endl << endl;
    }

    else
    {
        cout << endl << endl;
        for(int i = 0; i < 3; i++)
            cout << "------------- STALEMATE - DRAW! -------------" << endl;
        cout << endl << endl;
    }
}

void ChessBoard::printMove(string currentPosition, string nextPosition)
{
    if(whiteTurn_)
        cout << "White's ";
    else
        cout << "Black's ";
    cout << pieceType(currentPosition) << " moves from " << currentPosition
    << " to " << nextPosition;
}

void ChessBoard::printCapture(string nextPosition)
{
    if(Board_[nextPosition] != nullptr)
    {
        cout << " taking ";
        if(whiteTurn_)
            cout << "Black's ";
        else
            cout << "White's ";
        cout << pieceType(nextPosition) << endl;
    }
    else
        cout << endl;
}

string ChessBoard::pieceType(string currentPosition)
{
    if(dynamic_cast<Pawn*>(Board_[currentPosition]))
        return "Pawn";
    else if(dynamic_cast<Rook*>(Board_[currentPosition]))
        return "Rook";
    else if(dynamic_cast<Queen*>(Board_[currentPosition]))
        return "Queen";
    else if(dynamic_cast<Knight*>(Board_[currentPosition]))
        return "Knight";
    else if(dynamic_cast<King*>(Board_[currentPosition]))
        return "King";
    else if(dynamic_cast<Bishop*>(Board_[currentPosition]))
        return "Bishop";
    else
        return "Empty";
}

Colour ChessBoard::getPlayerTurn()
{
    return (whiteTurn_ == true ? Colour::White : Colour::Black);
}

string ChessBoard::getMyKingPosition()
{
    return (whiteTurn_ == true ? whiteKingPosition_ : blackKingPosition_);

}
