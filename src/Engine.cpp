#include "Engine.hpp"

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

#include "Board.hpp"
int PawnValue = 100;
int KnightValue = 300;
int BishopValue = 300;
int RookValue = 500;
int QueenValue = 900;

std::pair<Position, Position> bestMove;
std::pair<Position, Position> tempBestMove;

const int WaitingTime = 2 * 1000;

uint64_t startTime;

Engine::Engine() { std::srand(time(0)); }

int
Engine::search(int depth, Board &board, int alpha, int beta, Color turn,
               bool firstLevel = 1) {
    if (depth == 0)
        return evaluate(board);

    auto moves = board.getLegalMoves();
    if (moves.empty()) {
        if (turn == WHITE) {
            if (board.WhiteInCheck)
                return -1e5;
            else
                return 0;
        } else {
            if (board.BlackInCheck)
                return 1e5;
            else
                return 0;
        }
    }
    std::random_shuffle(moves.begin(), moves.end());
    int ans = 0;
    if (turn == WHITE) {
        int mx = -1e9;
        for (auto move : moves) {
            board.performMove(move.first, move.second);
            int dist =
                std::abs(board.WhiteKing.row - board.BlackKing.row) +
                std::abs(board.WhiteKing.column + board.BlackKing.column);
            int evaluation =
                search(depth - 1, board, alpha, beta, BLACK, 0) - dist;
            board.reverseMove();

            if (SDL_GetTicks() - startTime >= WaitingTime) {
                tempBestMove.first.init(-1, -1);
                tempBestMove.second.init(-1, -1);
                return 0;
            }

            if (evaluation > mx && firstLevel) {
                tempBestMove = move;
            }
            mx = std::max(mx, evaluation);
            alpha = std::max(alpha, evaluation);
            if (beta <= alpha)
                break;
        }
        return mx;
    } else {
        int mn = 1e9;
        for (auto move : moves) {
            board.performMove(move.first, move.second);

            int dist =
                std::abs(board.WhiteKing.row - board.BlackKing.row) +
                std::abs(board.WhiteKing.column + board.BlackKing.column);
            int evaluation =
                search(depth - 1, board, alpha, beta, WHITE, 0) + dist;
            board.reverseMove();

            if (SDL_GetTicks() - startTime >= WaitingTime) {
                tempBestMove.first.init(-1, -1);
                tempBestMove.second.init(-1, -1);
                return 0;
            }
            if (evaluation < mn && firstLevel)
                tempBestMove = move;
            mn = std::min(mn, evaluation);
            beta = std::min(beta, evaluation);
            if (beta <= alpha)
                break;
        }
        return mn;
    }
}
int
Engine::evaluate(Board &board) {
    int whiteEval = 0;
    int blackEval = 0;
    for (int row = 0; row < 8; row++) {
        for (int column = 0; column < 8; column++) {
            int *add = &whiteEval;
            if (board.board[row][column]->color == BLACK)
                add = &blackEval;
            switch (board.board[row][column]->type) {
                case PAWN:
                    *add += PawnValue;
                    break;
                case KNIGHT:
                    *add += KnightValue;
                    break;
                case QUEEN:
                    *add += QueenValue;
                    break;
                case BISHOP:
                    *add += BishopValue;
                    break;
                case ROOK:
                    *add += RookValue;
                    break;

                default:
                    break;
            }
        }
    }
    int evaluation = whiteEval - blackEval;
    return evaluation;
}

std::pair<Position, Position>
Engine::getBest(Board &board) {
    int depth = 1;
    bestMove.first.init(-1, -1);
    bestMove.second.init(-1, -1);
    startTime = SDL_GetTicks();
    while (1) {
        tempBestMove.first.init(5, 6);
        tempBestMove.second.init(5, 6);
        search(depth++, board, -1e9, 1e9, board.turn);
        if (~tempBestMove.first.row)
            bestMove = tempBestMove;
        else
            break;
    }
    std::cout << depth << std::endl;

    return bestMove;
}