#include <iostream>

using namespace std;

void whiteCastling();
void pawnPromotion();
void enPassant();
void stalemate();

ChessBoard cb;

void test()
{
    stalemate();

    cb.resetBoard();
    pawnPromotion();

    cb.resetBoard();
    whiteCastling();

    cb.resetBoard();
    enPassant();

}

void whiteCastling()
{
    cout << "===========================" << endl;
    cout << "White castling" << endl;
    cout << "===========================" << endl;
    cout << endl;

    cb.submitMove("E2", "E4");
    cout << endl;

    cb.submitMove("E7", "E5");
    cout << endl;

    cb.submitMove("G1", "F3");
    cout << endl;

    cb.submitMove("B8", "C6");
    cout << endl;

    cb.submitMove("F1", "B5");
    cout << endl;

    cb.submitMove("A7", "A6");
    cout << endl;

    cb.submitMove("Castling", "H1");
    cout << endl;
    cout << endl;
}

void pawnPromotion()
{
    cout << "===========================" << endl;
    cout << "White Pawn promotion" << endl;
    cout << "===========================" << endl;
    cout << endl;

    cb.resetBoard();
    cout << endl;

    cb.submitMove("E2", "E4");
    cout << endl;

    cb.submitMove("D7", "D5");
    cout << endl;

    cb.submitMove("E4", "D5");
    cout << endl;

    cb.submitMove("G8", "F6");
    cout << endl;

    cb.submitMove("F1", "C4");
    cout << endl;

    cb.submitMove("C7", "C6");
    cout << endl;

    cb.submitMove("D5", "C6");
    cout << endl;

    cb.submitMove("D8", "B6");
    cout << endl;

    cb.submitMove("C6", "B7");
    cout << endl;

    cb.submitMove("B6", "B5");
    cout << endl;

    cb.submitMove("B7", "C8");
    cout << endl;
    cout << endl;
}

void enPassant()
{
    cout << "===========================" << endl;
    cout << "En-passant testing" << endl;
    cout << "===========================" << endl;
    cout << endl;

    cb.submitMove("C2", "C4");
    cout << endl;

    cb.submitMove("C7", "C5");
    cout << endl;

    cb.submitMove("D2", "D4");
    cout << endl;

    cb.submitMove("F7", "F6");
    cout << endl;

    cb.submitMove("D4", "D5");
    cout << endl;

    cb.submitMove("G7", "G6");
    cout << endl;

    cb.submitMove("D5", "C6");
    cout << endl;
    cout << endl;
}

void stalemate()
{
    cout << "===========================" << endl;
    cout << "Stalemate testing" << endl;
    cout << "===========================" << endl;
    cout << endl;

    cb.submitMove("E2", "E3");
    cout << endl;

    cb.submitMove("A7", "A5");
    cout << endl;

    cb.submitMove("D1", "H5");
    cout << endl;

    cb.submitMove("A8", "A6");
    cout << endl;

    cb.submitMove("H5", "A5");
    cout << endl;

    cb.submitMove("H7", "H5");
    cout << endl;

    cb.submitMove("H2", "H4");
    cout << endl;

    cb.submitMove("A6", "H6");
    cout << endl;

    cb.submitMove("A5", "C7");
    cout << endl;

    cb.submitMove("F7", "F6");
    cout << endl;

    cb.submitMove("C7", "D7");
    cout << endl;

    cb.submitMove("E8", "F7");
    cout << endl;

    cb.submitMove("D7", "B7");
    cout << endl;

    cb.submitMove("D8", "D3");
    cout << endl;

    cb.submitMove("B7", "B8");
    cout << endl;

    cb.submitMove("D3", "H7");
    cout << endl;

    cb.submitMove("B8", "C8");
    cout << endl;

    cb.submitMove("F7", "G6");
    cout << endl;

    cb.submitMove("C8", "E6");
    cout << endl;

    cb.submitMove("F6", "F5");
    cout << endl;
    cout << endl;
}
