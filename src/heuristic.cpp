#include "heuristic.hpp"

int Heuristic::Minimax_Heuristic(Board board, int maxPlayer) {
    int minPlayer = (maxPlayer == WHITE)
                    ? BLACK
                    : WHITE;

    int pW = 2 * (board.score[maxPlayer] + board.score[minPlayer]);
    int cW = 5000;
    int clW = 2000;
    int fW = 200;
    int eW = 50;
    int mW = 10 * (100 - (board.score[maxPlayer] + board.score[minPlayer]));

    double pieceScore = (100.0 * board.score[maxPlayer]) / (board.score[maxPlayer] + board.score[minPlayer]);

    int maxCorner = 0, minCorner = 0;
    if (board.board[0][0] == maxPlayer) {
        maxCorner++;
    } else if (board.board[0][0] == minPlayer) {
        minCorner++;
    }
    if (board.board[0][BOARDSIZE - 1] == maxPlayer) {
        maxCorner++;
    } else if (board.board[0][BOARDSIZE - 1] == minPlayer) {
        minCorner++;
    }
    if (board.board[BOARDSIZE - 1][0] == maxPlayer) {
        maxCorner++;
    } else if (board.board[BOARDSIZE - 1][0] == minPlayer) {
        minCorner++;
    }
    if (board.board[BOARDSIZE - 1][BOARDSIZE - 1] == maxPlayer) {
        maxCorner++;
    } else if (board.board[BOARDSIZE - 1][BOARDSIZE - 1] == minPlayer) {
        minCorner++;
    }
    double cornerScore = 25.0 * (maxCorner - minCorner);

    int maxEdge = 0, minEdge = 0;
    int maxFrontier = 0, minFrontier = 0;
    for (int i = 0; i < BOARDSIZE; i++) {
        for (int j = 0; j < BOARDSIZE; j++) {
            if (i == 0 || i == BOARDSIZE - 1 || j == 0 || j == BOARDSIZE - 1) {
                if (board.board[i][j] == maxPlayer) {
                    maxEdge++;
                } else if (board.board[i][j] == minPlayer) {
                    minEdge++;
                }
            } else if (board.board[i][j] != '0') {
                if (board.OnFrontier(i, j)) {
                    if (board.board[i][j] == maxPlayer) {
                        maxFrontier++;
                    } else if (board.board[i][j] == minPlayer) {
                        minFrontier++;
                    }
                }
            }
        }
    }

    double edgeScore = 100.0 * maxEdge / (maxEdge + minEdge);
    double frontierScore = -100.0 * (maxFrontier - minFrontier); //Do not want frontier piece

    vector<Board::Move> maxLegalMove = board.LegalMoves(maxPlayer);
    vector<Board::Move> minLegalMove = board.LegalMoves(minPlayer);
    double moveScore = 100.0 * maxLegalMove.size() / (maxLegalMove.size() + minLegalMove.size());

    int cornerLoss = 0;
    for (auto &minMove : minLegalMove) {
        if (minMove.grid.y == 0 && minMove.grid.x == 0) {
            cornerLoss++;
        } else if (minMove.grid.y == 0 && minMove.grid.x == BOARDSIZE - 1) {
            cornerLoss++;
        } else if (minMove.grid.y == BOARDSIZE - 1 && minMove.grid.x == 0) {
            cornerLoss++;
        } else if (minMove.grid.y == BOARDSIZE - 1 && minMove.grid.x == BOARDSIZE - 1) {
            cornerLoss++;
        }
    }
    double cornerLossScore = -25.0 * cornerLoss;

    return int(ceil(pW * pieceScore + cW * cornerScore + clW * cornerLossScore +
                    eW * edgeScore + fW * frontierScore + mW * moveScore - 0.5));
}

