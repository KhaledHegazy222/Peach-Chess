#pragma once
#include <set>

#include "Game.hpp"
#include "Position.hpp"

enum Color { BLACK, WHITE };
enum PieceType { EMPTY, KING, QUEEN, KNIGHT, BISHOP, ROOK, PAWN };

class Board;

class Piece {
   protected:
    SDL_Texture *tex;

   public:
    Position position;
    Color color;
    PieceType type;

    Piece() {}
    Piece(int row, int column) {
        position.row = row;
        position.column = column;
        type = EMPTY;
    }

    std::set<std::pair<Position, Position>> virtual getLegalMoves(
        Board &board) {
        std::set<std::pair<Position, Position>> ret;
        return ret;
    }

    std::set<Position> virtual attack(Board &board) {
        std::set<Position> ret;
        return ret;
    }
    void virtual render(SDL_Rect *src, SDL_Rect *dest) {}
};