EXE = chess

OBJ = ChessMain.o ChessBoard.o Piece.o Rook.o Knight.o\
Bishop.o Queen.o King.o Pawn.o

CXX = g++
CXXFLAGS = -Wall -g -std=c++11

$(EXE): $(OBJ)
	$(CXX) $(OBJ) -o $@

ChessMain.o: ChessMain.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

ChessBoard.o: ChessBoard.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean

clean :
	rm -f $(OBJ) $(EXE)
