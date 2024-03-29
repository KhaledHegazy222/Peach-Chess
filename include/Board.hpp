#pragma once
#include <set>
#include <stack>
#include <vector>

#include "Bishop.hpp"
#include "King.hpp"
#include "Knight.hpp"
#include "Pawn.hpp"
#include "Piece.hpp"
#include "Queen.hpp"
#include "Rook.hpp"

class BoardInfo {
   public:
    BoardInfo(bool CWKS, bool CWQS, bool CBKS, bool CBQS, bool WC, bool BC,
              Position eP);

    bool CastleWhiteKingSide = false;
    bool CastleWhiteQueenSide = false;
    bool CastleBlackKingSide = false;
    bool CastleBlackQueenSide = false;
    bool WhiteInCheck = false;
    bool BlackInCheck = false;
    Position enPassant;
};

class Board {
   private:
    Piece *MovingPiece = NULL;
    std::set<std::pair<Position, Position>> availableMoves;

    std::stack<std::pair<Piece *, Piece *>> movesList;
    std::stack<BoardInfo> info;

   public:
    Position WhiteKing;
    Position BlackKing;
    bool isAttacked(Position cell);

    std::vector<std::vector<Piece *>> board;
    Color turn;
    Position enPassant;

    bool CastleWhiteKingSide = false;
    bool CastleWhiteQueenSide = false;
    bool CastleBlackKingSide = false;
    bool CastleBlackQueenSide = false;

    bool WhiteInCheck = false;
    bool BlackInCheck = false;

    Board();
    Board(const Board &other);
    ~Board();

    void init(const char *fen);
    void update();
    void render();
    void clear();

    void hold(int row, int column);
    void release(int row, int column);
    void performMove(Position from, Position to);
    void reverseMove();

    std::set<Position> attackedSpots();
    static bool validPosition(int row, int column);
    std::vector<std::pair<Position, Position>> getLegalMoves();
};