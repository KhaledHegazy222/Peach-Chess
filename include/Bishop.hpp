#pragma once
#include "Piece.hpp"
#include "Board.hpp"
#include "TextureManage.hpp"

class Bishop : public Piece {

public:

	Bishop();
	Bishop(int row, int column, Color _color);
	std::set<std::pair<Position, Position>> getLegalMoves(Board& board);
	std::set<Position> attack(Board& board);
	void render(SDL_Rect* src, SDL_Rect* dest);

};
