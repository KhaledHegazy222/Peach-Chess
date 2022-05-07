#include "Queen.hpp"


Queen::Queen() {
}

Queen::Queen(int row, int column, Color _color) {
	position.row = row;
	position.column = column;

	type = QUEEN;
	color = _color;
	if (color == WHITE)
		tex = TextureManager::loadTexture("../assets/WQueen.png");
	else
		tex = TextureManager::loadTexture("../assets/BQueen.png");
}


std::set<std::pair<Position, Position>> Queen::getLegalMoves(Board& board) {
	std::set<std::pair<Position, Position>> moves;
	for (int i = 1;i < 8;i++) {
		int row = position.row + i;
		int column = position.column + i;
		Position to(row, column);
		if (!Board::validPosition(row, column) || (board.board[row][column]->type != EMPTY && board.board[row][column]->color == color))
			break;
		moves.insert({ position,to });
		if (board.board[row][column]->type != EMPTY)
			break;
	}
	for (int i = 1;i < 8;i++) {
		int row = position.row - i;
		int column = position.column + i;
		Position to(row, column);
		if (!Board::validPosition(row, column) || (board.board[row][column]->type != EMPTY && board.board[row][column]->color == color))
			break;
		moves.insert({ position,to });
		if (board.board[row][column]->type != EMPTY)
			break;
	}
	for (int i = 1;i < 8;i++) {
		int row = position.row + i;
		int column = position.column - i;
		Position to(row, column);
		if (!Board::validPosition(row, column) || (board.board[row][column]->type != EMPTY && board.board[row][column]->color == color))
			break;
		moves.insert({ position,to });
		if (board.board[row][column]->type != EMPTY)
			break;
	}
	for (int i = 1;i < 8;i++) {
		int row = position.row - i;
		int column = position.column - i;
		Position to(row, column);
		if (!Board::validPosition(row, column) || (board.board[row][column]->type != EMPTY && board.board[row][column]->color == color))
			break;
		moves.insert({ position,to });
		if (board.board[row][column]->type != EMPTY)
			break;
	}
	for (int i = 1;i < 8;i++) {
		int row = position.row + i;
		int column = position.column;
		Position to(row, column);
		if (!Board::validPosition(row, column) || (board.board[row][column]->type != EMPTY && board.board[row][column]->color == color))
			break;
		moves.insert({ position,to });
		if (board.board[row][column]->type != EMPTY)
			break;
	}
	for (int i = 1;i < 8;i++) {
		int row = position.row;
		int column = position.column + i;
		Position to(row, column);
		if (!Board::validPosition(row, column) || (board.board[row][column]->type != EMPTY && board.board[row][column]->color == color))
			break;
		moves.insert({ position,to });
		if (board.board[row][column]->type != EMPTY)
			break;
	}
	for (int i = 1;i < 8;i++) {
		int row = position.row - i;
		int column = position.column;
		Position to(row, column);
		if (!Board::validPosition(row, column) || (board.board[row][column]->type != EMPTY && board.board[row][column]->color == color))
			break;
		moves.insert({ position,to });
		if (board.board[row][column]->type != EMPTY)
			break;
	}
	for (int i = 1;i < 8;i++) {
		int row = position.row;
		int column = position.column - i;
		Position to(row, column);
		if (!Board::validPosition(row, column) || (board.board[row][column]->type != EMPTY && board.board[row][column]->color == color))
			break;
		moves.insert({ position,to });
		if (board.board[row][column]->type != EMPTY)
			break;
	}
	return moves;
}

std::set<Position> Queen::attack(Board& board) {
	auto moves = this->getLegalMoves(board);
	std::set<Position> attackSpots;
	for (auto move : moves)
		attackSpots.insert(move.second);

	return attackSpots;
}

void Queen::render(SDL_Rect* src, SDL_Rect* dest) {
	TextureManager::drawTexture(tex, src, dest);
}
