#include "Knight.hpp"

#include "Board.hpp"

Knight::Knight() {}

Knight::Knight(int row, int column, Color _color) {
    position.row = row;
    position.column = column;
    type = KNIGHT;
    color = _color;
    if (color == WHITE)
        tex = TextureManager::loadTexture("../assets/WKnight.png");
    else
        tex = TextureManager::loadTexture("../assets/BKnight.png");
}

std::set<std::pair<Position, Position>>
Knight::getLegalMoves(Board &board) {
    std::set<std::pair<Position, Position>> moves;
    for (auto dx : {-2, -1, 1, 2}) {
        for (auto dy : {-2, -1, 1, 2}) {
            if (std::abs(dx) == std::abs(dy))
                continue;
            int row = position.row + dy;
            int column = position.column + dx;
            if (!Board::validPosition(row, column) ||
                (board.board[row][column]->type != EMPTY &&
                 board.board[row][column]->color == color))
                continue;
            Position to(row, column);
            moves.insert({position, to});
        }
    }

    return moves;
}

std::set<Position>
Knight::attack(Board &board) {
    auto moves = this->getLegalMoves(board);
    std::set<Position> attackSpots;
    for (auto move : moves) attackSpots.insert(move.second);

    return attackSpots;
}
void
Knight::render(SDL_Rect *src, SDL_Rect *dest) {
    TextureManager::drawTexture(tex, src, dest);
}
