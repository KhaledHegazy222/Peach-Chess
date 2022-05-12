#include "Board.hpp"
#include <iostream>

SDL_Texture* BTile;
SDL_Texture* WTile;
SDL_Texture* MoveTile;
SDL_Texture* MoveCircle;
SDL_Texture* TakeTile;
SDL_Texture* WLastMove;
SDL_Texture* BLastMove;
SDL_Texture* CheckTile;

Piece* clone(Piece* cell);

Board::Board() {
	board.resize(8);
	for (int i = 0;i < 8;i++)
		board[i].resize(8);
}

Board::Board(const Board& other) {
	board.resize(8);
	for (int i = 0;i < 8;i++)
		board[i].resize(8);
	for (int row = 0;row < 8;row++) {
		for (int column = 0;column < 8;column++) {
			switch (other.board[row][column]->type){
			case EMPTY:
				board[row][column] = new Piece(row, column);
				break;
			case KING:
				board[row][column] = new King(row, column, other.board[row][column]->color);
				break;
			case QUEEN:
				board[row][column] = new Queen(row, column, other.board[row][column]->color);
				break;
			case KNIGHT:
				board[row][column] = new Knight(row, column, other.board[row][column]->color);
				break;
			case BISHOP:
				board[row][column] = new Bishop(row, column, other.board[row][column]->color);
				break;
			case ROOK:
				board[row][column] = new Rook(row, column, other.board[row][column]->color);
				break;
			case PAWN:
				board[row][column] = new Pawn(row, column, other.board[row][column]->color);
				break;
			default:
				break;
			}
		}
	}
	turn = other.turn;
	enPassant = other.enPassant;
	WhiteKing = other.WhiteKing;
	BlackKing = other.BlackKing;
	CastleBlackKingSide = other.CastleBlackKingSide;
	CastleBlackQueenSide = other.CastleBlackQueenSide;
	CastleWhiteKingSide = other.CastleWhiteKingSide;
	CastleWhiteQueenSide = other.CastleWhiteQueenSide;
}

Board::~Board() {
}

void Board::init(const char* fen) {

	BTile = TextureManager::loadTexture("../assets/BTile.png");
	WTile = TextureManager::loadTexture("../assets/WTile.png");
	MoveTile = TextureManager::loadTexture("../assets/MoveTile.png");
	MoveCircle = TextureManager::loadTexture("../assets/MoveCircle.png");
	TakeTile = TextureManager::loadTexture("../assets/TakeTile.png");
	WLastMove = TextureManager::loadTexture("../assets/WLastMove.png");
	BLastMove = TextureManager::loadTexture("../assets/BLastMove.png");
	CheckTile = TextureManager::loadTexture("../assets/CheckTile.png");



	int row = 0, column = 0;
	int i;
	for (i = 0;fen[i] != '\0' && row != 8;i++) {
		if (fen[i] == '/') {
			continue;
		}
		else if (SDL_isdigit(fen[i])) {
			for (int j = 0;j < fen[i] - '0';j++) {
				board[row][column++] = new Piece(row,column);
			}
		}
		else {
			switch (fen[i]) {
			case 'K':
				board[row][column] = new King(row, column, WHITE);
				WhiteKing.init(row, column);
				break;
			case 'k':
				board[row][column] = new King(row, column, BLACK);
				BlackKing.init(row, column);
				break;
			case 'Q':
				board[row][column] = new Queen(row, column, WHITE);
				break;
			case 'q':
				board[row][column] = new Queen(row, column, BLACK);
				break;
			case 'N':
				board[row][column] = new Knight(row, column, WHITE);
				break;
			case 'n':
				board[row][column] = new Knight(row, column, BLACK);
				break;
			case 'B':
				board[row][column] = new Bishop(row, column, WHITE);
				break;
			case 'b':
				board[row][column] = new Bishop(row, column, BLACK);
				break;
			case 'R':
				board[row][column] = new Rook(row, column, WHITE);
				break;
			case 'r':
				board[row][column] = new Rook(row, column, BLACK);
				break;
			case 'P':
				board[row][column] = new Pawn(row, column, WHITE);
				break;
			case 'p':
				board[row][column] = new Pawn(row, column, BLACK);
				break;
			default:
				board[row][column] = new Piece(row, column);
				break;
			}
			column++;
		}
		row += column / 8;
		column %= 8;
		
	}
	

	i++;
	if (fen[i] == 'w')
		turn = WHITE;
	else if (fen[i] == 'b')
		turn = BLACK;

	for (;fen[i] != '\0';i++) {
		if (fen[i] == 'K')
			CastleWhiteKingSide = true;
		else if (fen[i] == 'Q')
			CastleWhiteQueenSide = true;
		else if (fen[i] == 'k')
			CastleBlackKingSide = true;
		else if (fen[i] == 'q')
			CastleBlackQueenSide = true;
		else if (SDL_isalpha(fen[i])) {
			enPassant.column = fen[i] - 'a';
			enPassant.row = '8' - fen[i + 1];
		}
		else if (fen[i] == '-')
			break;
	}
}

void Board::update() {
}

void Board::render() {
	
	// Render Chess board Tiles
	for (int row = 0;row < 8;row++) {
		for (int column = 0;column < 8;column++) {
			SDL_Rect dest;
			dest.x = Game::PaddingLeft + row * Game::TileDimension;
			dest.y = Game::PaddingTop + column * Game::TileDimension;
			dest.w = dest.h = Game::TileDimension;
			TextureManager::drawTexture((row + column) % 2 == 0 ? WTile : BTile, NULL, &dest);
		}
	}

	// Render Last Played Move
	if (movesList.size() && validPosition(movesList.top().first->position.row, movesList.top().first->position.column)) {
		SDL_Rect dest;
		dest.x = Game::PaddingLeft + movesList.top().first->position.column * Game::TileDimension;
		dest.y = Game::PaddingTop + movesList.top().first->position.row * Game::TileDimension;
		dest.h = dest.w = Game::TileDimension;
		TextureManager::drawTexture((movesList.top().first->position.row + movesList.top().first->position.column) % 2 == 0 ? WLastMove : BLastMove, NULL, &dest);
	}
	if (movesList.size() && validPosition(movesList.top().second->position.row, movesList.top().second->position.column)) {
		SDL_Rect dest;
		dest.x = Game::PaddingLeft + movesList.top().second->position.column * Game::TileDimension;
		dest.y = Game::PaddingTop + movesList.top().second->position.row * Game::TileDimension;
		dest.h = dest.w = Game::TileDimension;
		TextureManager::drawTexture((movesList.top().second->position.row + movesList.top().second->position.column) % 2 == 0 ? WLastMove : BLastMove, NULL, &dest);
	}


	// Render Available Moves
	for (auto move : availableMoves) {
		SDL_Rect dest;
		if (board[move.second.row][move.second.column]->type != EMPTY && board[move.second.row][move.second.column]->color != turn) {
			dest.x = Game::PaddingLeft + move.second.column * Game::TileDimension;
			dest.y = Game::PaddingTop + move.second.row * Game::TileDimension;
			dest.h = dest.w = Game::TileDimension;
			TextureManager::drawTexture(TakeTile, NULL, &dest);
		}
		else {
			dest.x = Game::PaddingLeft + move.second.column * Game::TileDimension + Game::TileDimension / 3;
			dest.y = Game::PaddingTop + move.second.row * Game::TileDimension + Game::TileDimension / 3;
			dest.h = dest.w = Game::TileDimension / 3;
			TextureManager::drawTexture(MoveCircle, NULL, &dest);
		}
	}
	if (WhiteInCheck) {
		SDL_Rect dest;
		dest.x = Game::PaddingLeft + WhiteKing.column * Game::TileDimension;
		dest.y = Game::PaddingTop + WhiteKing.row * Game::TileDimension;
		dest.h = dest.w = Game::TileDimension;
		TextureManager::drawTexture(CheckTile, NULL, &dest);
	}


	if (BlackInCheck) {
		SDL_Rect dest;
		dest.x = Game::PaddingLeft + BlackKing.column * Game::TileDimension;
		dest.y = Game::PaddingTop + BlackKing.row * Game::TileDimension;
		dest.h = dest.w = Game::TileDimension;
		TextureManager::drawTexture(CheckTile, NULL, &dest);
	}

	// Render Chess Pieces
	for (int row = 0;row < 8;row++) {
		for (int column = 0;column < 8;column++) {
			SDL_Rect dest;
			dest.x = Game::PaddingLeft + column * Game::TileDimension;
			dest.y = Game::PaddingTop + row * Game::TileDimension;
			dest.w = dest.h = Game::TileDimension;
			board[row][column]->render(NULL,&dest);
		}
	}

	// Render Moving Piece
	if (MovingPiece != NULL) {
		
		SDL_Rect dest;
		SDL_GetMouseState(&(dest.x), &(dest.y));
		dest.x -= Game::TileDimension / 2;
		dest.y -= Game::TileDimension / 2;
		dest.h = dest.w = Game::TileDimension;
		MovingPiece->render(NULL, &dest);
	}
}

void Board::clear() {
	while (movesList.size()) {
		reverseMove();
	}
	for (int row = 0;row < 8;row++) {
		for (int column = 0;column < 8;column++) {
			delete board[row][column];
		}
	}
}

void Board::hold(int row,int column) {
	MovingPiece = NULL;
	if (!validPosition(row, column) || board[row][column]->type == EMPTY || board[row][column]->color != turn) {
			
		return;
	}

	MovingPiece = clone(board[row][column]);
	auto moves = MovingPiece->getLegalMoves(*this);
	
	for (auto move : moves) {
		
		performMove(move.first, move.second);
		turn = (turn == WHITE ? BLACK : WHITE);
		if(turn == WHITE ? !WhiteInCheck : !BlackInCheck)
			availableMoves.insert(move);
		turn = (turn == WHITE ? BLACK : WHITE);
		reverseMove();
	}
	delete board[row][column];
	board[row][column] = new Piece(row, column);	
}

void Board::release(int row,int column) {
	
	if (MovingPiece == NULL)
		return;
	Position from(MovingPiece->position.row, MovingPiece->position.column);
	Position to(row, column);
	if (!validPosition(row, column) || availableMoves.find({from,to}) == availableMoves.end()) {
		availableMoves.clear();
		delete board[from.row][from.column];
		board[from.row][from.column] = MovingPiece;
		MovingPiece = NULL;
		return;
	}


	delete board[from.row][from.column];
	board[from.row][from.column] = MovingPiece;
	MovingPiece = NULL;
	


	performMove(from, to);
	
	availableMoves.clear();

}

bool Board::validPosition(int row, int column) {
	return row >= 0 && row < 8 && column >= 0 && column < 8;
}

std::set<Position> Board::attackedSpots() {
	std::set<Position> result;
	for (int row = 0;row < 8;row++) {
		for (int column = 0;column < 8;column++) {
			if (board[row][column]->type != EMPTY && board[row][column]->color != turn) {
				auto current = board[row][column]->attack(*this);
				result.insert(current.begin(), current.end());
			}
		}
	}
	return result;
}

bool Board::isAttacked(Position cell) {
	
	for (int dx : {-2, -1, 1, 2}) {
		for (int dy : {-2, -1, 1, 2}) {
			if (std::abs(dx) == std::abs(dy))
				continue;
			int row = cell.row + dy;
			int column = cell.column + dx;
			if (validPosition(row,column) && board[row][column]->type == KNIGHT && board[row][column]->color != turn) {
				return true;
			}
		}
	}

	for (int i = -1;i <= 1;i++) {
		for (int j = -1;j <= 1;j++) {
			if (!i && !j)
				continue;
			
			int row = cell.row + i;
			int column = cell.column + j;
			while (validPosition(row, column)) {
				if(board[row][column]->type != EMPTY) {
					if (board[row][column]->color == turn)
						break;
					else if (i && j && (board[row][column]->type == QUEEN || board[row][column]->type == BISHOP)) 
						return true;
					else if ((!i || !j) && (board[row][column]->type == QUEEN || board[row][column]->type == ROOK)) 
						return true;
					else
						break;
				}
				row += i;
				column += j;
			}
		}
	}
	for (int i = -1;i <= 1;i++) {
		for (int j = -1;j <= 1;j++) {
			int row = cell.row + i;
			int column = cell.column + j;
			if (validPosition(row, column) && board[row][column]->type == KING && board[row][column]->color != turn)
				return true;
		}
	}
	for (int i = -1;i <= 1;i += 2) {
		int row = cell.row - (turn == WHITE ? 1 : -1);
		int column = cell.column + i;
		if (validPosition(row, column) && board[row][column]->type == PAWN && board[row][column]->color != turn)
			return true;
		
	}
	return false;
}

void Board::performMove(Position from, Position to) {

	Piece* LastMoveFrom = clone(board[from.row][from.column]);
	Piece* LastMoveTo = clone(board[to.row][to.column]);
	movesList.push({ LastMoveFrom,LastMoveTo });

	BoardInfo currentInfo(CastleWhiteKingSide, CastleWhiteQueenSide, CastleBlackKingSide, CastleBlackQueenSide,WhiteInCheck,BlackInCheck,enPassant);
	info.push(currentInfo);




	// Performing En Passant Move
	if (board[from.row][from.column]->type == PAWN && enPassant.row == from.row && to.column == enPassant.column) {
		delete board[enPassant.row][enPassant.column];
		board[enPassant.row][enPassant.column] = new Piece(enPassant.row, enPassant.column);
	}

	// Updating En Passant Last Move
	if (board[from.row][from.column]->type == PAWN && abs(from.row - to.row) == 2) 
		enPassant = to;
	else
		enPassant.init(-1, -1);

	


	delete board[to.row][to.column];
	board[to.row][to.column] = board[from.row][from.column];
	board[to.row][to.column]->position = to;
	board[from.row][from.column] = new Piece(from.row, from.column);


	if ((to.row == 0 || to.row == 7) && board[to.row][to.column]->type == PAWN) {
		delete board[to.row][to.column];
		board[to.row][to.column] = new Queen(to.row, to.column,turn);
	}
			

	// Castling
	if (board[to.row][to.column]->type == KING && std::abs(from.column - to.column) > 1) {
		if (to.column == 6) {
			Position RookFrom(from.row, to.column + 1);
			Position RookTo(to.row, to.column - 1);
			performMove(RookFrom, RookTo);
		}
		else {
			Position RookFrom(from.row, to.column - 2);
			Position RookTo(to.row, to.column + 1);
			performMove(RookFrom, RookTo);
		}

		movesList.pop();
		info.pop();
		turn = (turn == WHITE ? BLACK : WHITE);
	}

	// Updating Castle Rules And Kings Positions
	if (board[to.row][to.column]->type == KING) {
		if (board[to.row][to.column]->color == WHITE) {
			WhiteKing = to;
			CastleWhiteKingSide = CastleWhiteQueenSide = false;
		}
		else {
			BlackKing = to;
			CastleBlackKingSide = CastleBlackQueenSide = false;
		}
	}


	if (board[7][7]->type != ROOK)
		CastleWhiteKingSide = false;
	if (board[7][0]->type != ROOK)
		CastleWhiteQueenSide = false;

	if (board[0][7]->type != ROOK)
		CastleBlackKingSide = false;
	if (board[0][0]->type != ROOK)
		CastleBlackQueenSide = false;

	auto temp = turn;
	turn = WHITE;
	WhiteInCheck = isAttacked(WhiteKing);
	turn = BLACK;
	BlackInCheck = isAttacked(BlackKing);
	turn = temp;
	turn = (turn == WHITE ? BLACK : WHITE);
}

void Board::reverseMove() {
	if (movesList.empty())
		return;



	Piece* from = movesList.top().first;
	Piece* to = movesList.top().second;
	movesList.pop();

	BoardInfo lastInfo = info.top();
	info.pop();
	CastleWhiteKingSide = lastInfo.CastleWhiteKingSide;
	CastleWhiteQueenSide = lastInfo.CastleWhiteQueenSide;
	CastleBlackKingSide = lastInfo.CastleBlackKingSide;
	CastleBlackQueenSide = lastInfo.CastleBlackQueenSide;
	WhiteInCheck = lastInfo.WhiteInCheck;
	BlackInCheck = lastInfo.BlackInCheck;
	enPassant = lastInfo.enPassant;


	if(from->type == PAWN && to->position.column != from->position.column && to->type == EMPTY){
		delete board[to->position.row - (from->color == WHITE ? -1 : 1)][to->position.column];
		board[to->position.row - (from->color == WHITE ? -1 : 1)][to->position.column] = new Pawn(to->position.row - (from->color == WHITE ? -1 : 1),to->position.column,(from->color == WHITE ? BLACK : WHITE));
	}

	delete board[from->position.row][from->position.column];
	delete board[to->position.row][to->position.column];

	board[from->position.row][from->position.column] = from;
	board[to->position.row][to->position.column] = to;
	

	// Castling
	if (board[from->position.row][from->position.column]->type == KING && std::abs(from->position.column - to->position.column) > 1) {
		if (to->position.column == 6) {
			Position RookFrom(from->position.row, 5);
			Position RookTo(from->position.row, 7);
			performMove(RookFrom, RookTo);
		}
		else {
			Position RookFrom(from->position.row, 3);
			Position RookTo(from->position.row, 0);
			performMove(RookFrom, RookTo);
		}

		movesList.pop();
		info.pop();
		turn = (turn == WHITE ? BLACK : WHITE);
	}

	// Updating Kings Positions
	if (board[from->position.row][from->position.column]->type == KING) {
		if (from->color == WHITE) {
			WhiteKing = from->position;
		}
		else {
			BlackKing = from->position;
		}
	}




	turn = (turn == WHITE ? BLACK : WHITE);
}

std::vector<std::pair<Position, Position>> Board::getLegalMoves() {
	
	std::vector<std::pair<Position, Position>> moves;
	for (int row = 0;row < 8;row++) {
		for (int column = 0;column < 8;column++) {
			if (board[row][column]->type != EMPTY && board[row][column]->color == turn) {
				auto current = board[row][column]->getLegalMoves(*this);
				for (auto move : current) {
					performMove(move.first, move.second);
					turn = (turn == WHITE ? BLACK : WHITE);
					if (turn == WHITE ? !WhiteInCheck : !BlackInCheck)
						moves.push_back(move);
					turn = (turn == WHITE ? BLACK : WHITE);
					reverseMove();
				}
			}
		}
	}
	return moves;
}

Piece* clone(Piece* cell) {
	switch (cell->type) {
	case EMPTY:
		return new Piece(cell->position.row, cell->position.column);
		break;
	case KING:
		return new King(cell->position.row, cell->position.column, cell->color);
		break;
	case QUEEN:
		return new Queen(cell->position.row, cell->position.column, cell->color);
		break;
	case KNIGHT:
		return new Knight(cell->position.row, cell->position.column, cell->color);
		break;
	case BISHOP:
		return new Bishop(cell->position.row, cell->position.column, cell->color);
		break;
	case ROOK:
		return new Rook(cell->position.row, cell->position.column, cell->color);
		break;
	case PAWN:
		return new Pawn(cell->position.row, cell->position.column, cell->color);
		break;
	default:
		return NULL;
		break;
	}
}

BoardInfo::BoardInfo(bool CWKS, bool CWQS, bool CBKS, bool CBQS,bool WC,bool BC,Position eP) {
	CastleWhiteKingSide = CWKS;
	CastleWhiteQueenSide = CWQS;
	CastleBlackKingSide = CBKS;
	CastleBlackQueenSide = CBQS;
	WhiteInCheck = WC;
	BlackInCheck = BC;
	enPassant = eP;
}
