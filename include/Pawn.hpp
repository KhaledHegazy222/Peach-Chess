#pragma once
#include "Board.hpp"
#include "Piece.hpp"
#include "TextureManage.hpp"

class Pawn : public Piece {
   public:
    Pawn();
    Pawn(int row, int column, Color _color);
    Pawn(Pawn *pawn);
    std::set<std::pair<Position, Position>> getLegalMoves(Board &board);
    std::set<Position> attack(Board &board);
    void render(SDL_Rect *src, SDL_Rect *dest);
};
