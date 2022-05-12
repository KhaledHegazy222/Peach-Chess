#include "Pawn.hpp"


Pawn::Pawn() {
}

Pawn::Pawn(int row, int column, Color _color) {
	position.row = row;
	position.column = column;
	type = PAWN;
	color = _color;
	if (color == WHITE)
		tex = TextureManager::loadTexture("../assets/WPawn.png");
	else
		tex = TextureManager::loadTexture("../assets/BPawn.png");
}


std::set<std::pair<Position, Position>> Pawn::getLegalMoves(Board& board) {
	std::set<std::pair<Position, Position>> moves;

	if (color == WHITE) {
		int row = position.row - 1;
		int column = position.column;
		if (Board::validPosition(row, column) && board.board[row][column]->type == EMPTY) {
			Position to(row, column);
			moves.insert({ position,to });
		}
		row = position.row - 2;
		column = position.column;
		if (row == 4 && board.board[row][column]->type == EMPTY && board.board[row + 1][column]->type == EMPTY){
			Position to(row, column);
			moves.insert({ position,to });
		}

		row = position.row - 1;
		column = position.column - 1;
		if (Board::validPosition(row, column) && board.board[row][column]->type != EMPTY && board.board[row][column]->color != color) {
			Position to(row, column);
			moves.insert({ position,to });
		}
		row = position.row - 1;
		column = position.column + 1;
		if (Board::validPosition(row, column) && board.board[row][column]->type != EMPTY && board.board[row][column]->color != color) {
			Position to(row, column);
			moves.insert({ position,to });
		}

		
	}
	else {
		int row = position.row + 1;
		int column = position.column;
		if (Board::validPosition(row, column) && board.board[row][column]->type == EMPTY) {
			Position to(row, column);
			moves.insert({ position,to });
		}
		row = position.row + 2;
		column = position.column;
		if (row == 3 && board.board[row][column]->type == EMPTY && board.board[row - 1][column]->type == EMPTY) {
			Position to(row, column);
			moves.insert({ position,to });
		}

		row = position.row + 1;
		column = position.column - 1;
		if (Board::validPosition(row, column) && board.board[row][column]->type != EMPTY && board.board[row][column]->color != color) {
			Position to(row, column);
			moves.insert({ position,to });
		}
		row = position.row + 1;
		column = position.column + 1;
		if (Board::validPosition(row, column) && board.board[row][column]->type != EMPTY && board.board[row][column]->color != color) {
			Position to(row, column);
			moves.insert({ position,to });
		}
	}
	
	if (position.row == board.enPassant.row && std::abs(position.column - board.enPassant.column) == 1) {
		Position to(position.row - (color == WHITE ? 1 : -1), board.enPassant.column);
		moves.insert({ position,to });
	}
	

	return moves;
}

std::set<Position> Pawn::attack(Board& board) {
	std::set<Position> attackSpots;
	int row = position.row + (color == WHITE ? -1 : 1);
	int column = position.column + 1;
	if (Board::validPosition(row, column) && board.board[row][column]->type != EMPTY && board.board[row][column]->color != color) {
		Position to(row, column);
		attackSpots.insert(to);
	}
	row = position.row + (color == WHITE ? -1 : 1);
	column = position.column - 1;
	if (Board::validPosition(row, column) && board.board[row][column]->type != EMPTY && board.board[row][column]->color != color) {
		Position to(row, column);
		attackSpots.insert(to);
	}

	return attackSpots;
}

void Pawn::render(SDL_Rect* src, SDL_Rect* dest) {
	TextureManager::drawTexture(tex, src, dest);
}

