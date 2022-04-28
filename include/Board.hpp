#pragma once
#include "Piece.hpp"
#include "MessageBoxManager.hpp"
#include <set>

class Board {
    private:
        Piece board[8][8];
        Piece BlackKing,WhiteKing;
        bool WhiteCastleKingSide = true;
        bool WhiteCastleQueenSide = true;
        bool BlackCastleKingSide = true;
        bool BlackCastleQueenSide = true;

        bool isChecked(Color cl);
        void performMove(Piece piece,int row,int col,int movement);
        void reverseMove(Piece from,Piece to);

    public:
    

        Color turn = White;

        Board();
        Board(int width,int height);
        void init(const char* fen);
        void handleEvents();
        void update();
        void render();
        void move(int row,int col);
        void release(int row,int col);

        static bool validPos(int row,int col);
        std::set<std::pair<int,int>>sudoLegalMoves(Piece piece,bool movement);
        std::set<std::pair<int,int>>attacked(Color cl);

        void promote(int row,int col);

};