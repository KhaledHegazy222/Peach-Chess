#include "King.hpp"


King::King() {
}

King::King(int row, int column, Color _color) {
	position.row = row;
	position.column = column;
	type = KING;
	color = _color;
	if (color == WHITE)
		tex = TextureManager::loadTexture("../assets/WKing.png");
	else
		tex = TextureManager::loadTexture("../assets/BKing.png");
}


std::set<std::pair<Position, Position>> King::getLegalMoves(Board& board) {
	std::set<std::pair<Position, Position>> moves;
	for (int i = -1;i <= 1;i++) {
		for (int j = -1;j <= 1;j++) {
			if (!i && !j)
				continue;
			int row = position.row + i;
			int column = position.column + j;
			Position to(row, column);
			if (!Board::validPosition(row, column) || (board.board[row][column]->type != EMPTY && board.board[row][column]->color == color))
				continue;
			moves.insert({ position,to });
		}
	}
	auto temp = board.turn;
	// Castling Rules
	if (color == WHITE) {
		board.turn = WHITE;
		if ((board.CastleWhiteKingSide && board.board[7][5]->type == EMPTY && board.board[7][6]->type == EMPTY) || (board.CastleWhiteQueenSide && board.board[7][3]->type == EMPTY && board.board[7][2]->type == EMPTY && board.board[7][1]->type == EMPTY)) {
			if (!board.isAttacked(board.board[7][4]->position)) {

				if (board.CastleWhiteKingSide && !board.isAttacked(board.board[7][4]->position) && board.board[7][5]->type == EMPTY && !board.isAttacked(board.board[7][5]->position) && board.board[7][6]->type == EMPTY && !board.isAttacked(board.board[7][6]->position)) {
					Position to(7, 6);
					moves.insert({ position,to });
				}
				if (board.CastleWhiteQueenSide && !board.isAttacked(board.board[7][4]->position) && board.board[7][3]->type == EMPTY && !board.isAttacked(board.board[7][3]->position) && board.board[7][2]->type == EMPTY && !board.isAttacked(board.board[7][2]->position) && board.board[7][1]->type == EMPTY) {
					Position to(7, 2);
					moves.insert({ position,to });
				}
			}
		}
	}	
	else {
		board.turn = BLACK;
		if ((board.CastleBlackKingSide && board.board[0][5]->type == EMPTY && board.board[0][6]->type == EMPTY) || (board.CastleBlackQueenSide && board.board[0][3]->type == EMPTY && board.board[0][2]->type == EMPTY)) {
			if (!board.isAttacked(board.board[0][4]->position)) {

				if (board.CastleBlackKingSide && board.board[0][5]->type == EMPTY && !board.isAttacked(board.board[0][5]->position) && board.board[0][6]->type == EMPTY && !board.isAttacked(board.board[0][6]->position)) {
					Position to(0, 6);
					moves.insert({ position,to });
				}
				if (board.CastleBlackQueenSide && board.board[0][3]->type == EMPTY && !board.isAttacked(board.board[0][3]->position) && board.board[0][2]->type == EMPTY && !board.isAttacked(board.board[0][2]->position) && board.board[0][1]->type == EMPTY) {
					Position to(0, 2);
					moves.insert({ position,to });
				}
			}
				
		}
	}
	board.turn = temp;


	return moves;
}

std::set<Position> King::attack(Board& board) {
	std::set<Position> attackSpots;
	for (int i = -1;i <= 1;i++) {
		for (int j = -1;j <= 1;j++) {
			if (!i && !j)
				continue;
			int row = position.row + i;
			int column = position.column + j;
			Position to(row, column);
			if (Board::validPosition(row, column) && board.board[row][column]->type != EMPTY && board.board[row][column]->color != color)
				attackSpots.insert(to);
		}
	}
	return attackSpots;

}

void King::render(SDL_Rect* src, SDL_Rect* dest) {
	TextureManager::drawTexture(tex, src, dest);
}

