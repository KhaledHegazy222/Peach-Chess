#include "Bishop.hpp"

Bishop::Bishop() {}

Bishop::Bishop(int row, int column, Color _color) {
    position.row = row;
    position.column = column;
    type = BISHOP;
    color = _color;
    if (color == WHITE)
        tex = TextureManager::loadTexture("../assets/WBishop.png");
    else
        tex = TextureManager::loadTexture("../assets/BBishop.png");
}

std::set<std::pair<Position, Position>>
Bishop::getLegalMoves(Board &board) {
    std::set<std::pair<Position, Position>> moves;
    for (int i = 1; i < 8; i++) {
        int row = position.row + i;
        int column = position.column + i;
        Position to(row, column);
        if (!Board::validPosition(row, column) ||
            (board.board[row][column]->type != EMPTY &&
             board.board[row][column]->color == color))
            break;
        moves.insert({position, to});
        if (board.board[row][column]->type != EMPTY)
            break;
    }
    for (int i = 1; i < 8; i++) {
        int row = position.row - i;
        int column = position.column + i;
        Position to(row, column);
        if (!Board::validPosition(row, column) ||
            (board.board[row][column]->type != EMPTY &&
             board.board[row][column]->color == color))
            break;
        moves.insert({position, to});
        if (board.board[row][column]->type != EMPTY)
            break;
    }
    for (int i = 1; i < 8; i++) {
        int row = position.row + i;
        int column = position.column - i;
        Position to(row, column);
        if (!Board::validPosition(row, column) ||
            (board.board[row][column]->type != EMPTY &&
             board.board[row][column]->color == color))
            break;
        moves.insert({position, to});
        if (board.board[row][column]->type != EMPTY)
            break;
    }
    for (int i = 1; i < 8; i++) {
        int row = position.row - i;
        int column = position.column - i;
        Position to(row, column);
        if (!Board::validPosition(row, column) ||
            (board.board[row][column]->type != EMPTY &&
             board.board[row][column]->color == color))
            break;
        moves.insert({position, to});
        if (board.board[row][column]->type != EMPTY)
            break;
    }
    return moves;
}

std::set<Position>
Bishop::attack(Board &board) {
    auto moves = this->getLegalMoves(board);
    std::set<Position> attackSpots;
    for (auto move : moves) attackSpots.insert(move.second);

    return attackSpots;
}
void
Bishop::render(SDL_Rect *src, SDL_Rect *dest) {
    TextureManager::drawTexture(tex, src, dest);
}
