#ifndef _GAME_CPP_
#define _GAME_CPP_

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <random>
#include <climits>
#include <ctime>
#include <cctype>
#include <unistd.h>
#include <cstdlib>
#include "game.h"
#include "board.h"
#include "const.h"

using namespace std;

void Game::setup(int gameType, int tLimit, string board_file) {
    switch (gameType) {
        case (0):
            humanPlayer[BLACK] = true;
            humanPlayer[WHITE] = false;
            break;
        case (1):
            humanPlayer[BLACK] = false;
            humanPlayer[WHITE] = true;
        default:
            throw;
    }
    firstPlayer = gameType;
    if (!board_file.empty()) {
        int state[BOARDSIZE][BOARDSIZE];
        int player;
        int i = 0, j = 0;
        fstream fs;

        fs.open(board_file.c_str(), fstream::in);
        char c;
        while ((c = fs.get()) != EOF) {
            if (c == '0' || c == '1' || c == '2') {
                state[i][j++] = c - '0';
                if (j == BOARDSIZE) {
                    j = 0;
                    ++i;
                    if (i == BOARDSIZE) {
                        break;
                    }
                }
            }
        }

        fs >> player;
        board = Board(state, player);
        timeLimit = tLimit;
        fs.close();
    } else {
        timeLimit = tLimit;
        board = Board();
    }
}

bool Game::RandomMove() {
    vector<Board::Move> m = board.LegalMoves(board.currentPlayer);
    if (!m.empty()) {
        Board::Move randMove = m[randomizer() % m.size()];
        board.PrintBoard(vector<Board::Move>(1, randMove), true, firstPlayer);
        board.ApplyMove(randMove);
        return board.SwitchPlayer(false);
    } else {
        cout << "Computer had to pass :(" << endl;
        return board.SwitchPlayer(true);
    }
}

//bool Game::HumanMove()
//  method to apply move to game based on human input
//  returns false if game in terminal state
bool Game::HumanMove() {
    int moveNum = -1;
    string in;
    vector<Board::Move> m = board.LegalMoves(board.currentPlayer);
    if (!m.empty()) {
        board.PrintBoard(m, false, firstPlayer);
        for (int i = 0; i < m.size(); i++) {
            cout << i << ": [" << m[i].grid.y << "," << m[i].grid.x << "]" << endl;
        }

        //make sure the user inputs a valid move
        while (!(moveNum < m.size() && moveNum >= 0)) {
            cout << "Your Move: " << endl;
            cin >> in;
            if (isdigit(in.c_str()[0]))
                moveNum = stoi(in);
        }
        board.ApplyMove(m[moveNum]);
        return board.SwitchPlayer(false);
    } else {
        cout << "Sorry, you have to pass :(" << endl;
        return board.SwitchPlayer(true);
    }
}

int Game::Minimax_heuristic(Board board) {
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

    return pW * pieceScore + cW * cornerScore + clW * cornerLossScore +
           eW * edgeScore + fW * frontierScore + mW * moveScore;
}

int Game::alphabetaPruning(Board board, int depth, int alpha, int beta, bool maxPlayer) {
    int a = alpha, b = beta;

    if ((float) (clock() - startTime) / CLOCKS_PER_SEC > STOPTIME * timeLimit) {
        timeout = true;
        return Minimax_heuristic(board);
    } else if (depth == 0) {
        return Minimax_heuristic(board);
    } else {
        depth--;
    }
    vector<Board::Move> m = board.LegalMoves(board.currentPlayer);

    if (m.empty()) {
        if (board.TerminalState(true)) {
            Board testBoard = board;
            testBoard.SwitchPlayer(false);
            return Minimax_heuristic(testBoard);
        } else {
            Board testBoard = board;
            testBoard.SwitchPlayer(true);
            return alphabetaPruning(testBoard, depth, alpha, beta, !maxPlayer);
        }
    }

    if (maxPlayer) {
        int value = INT_MIN;
        for (auto &move : m) {
            Board testBoard = board;
            board.ApplyMove(move);
            board.SwitchPlayer(false);
            int score = alphabetaPruning(testBoard, depth, alpha, beta, false);
            value = value > score ? value : score;

            if (value >= b) {
                return value;
            }
            a = a > value ? a : value;
        }
        return value;
    } else {
        int value = INT_MAX;
        for (auto &move : m) {
            Board testBoard = board;
            testBoard.ApplyMove(move);
            testBoard.SwitchPlayer(false);
            int score = alphabetaPruning(testBoard, depth, alpha, beta, true);
            value = value < score ? value : score;

            if (value <= a) {
                return value;
            }
            b = b < value ? b : value;
        }
        return value;
    }
}

bool Game::AutoMove() {
    startTime = clock();
    maxPlayer = board.currentPlayer;

    vector<Board::Move> legalMove = board.LegalMoves(maxPlayer);

    if (legalMove.empty()) {
        cout << "Pass" << endl;
        return board.SwitchPlayer(true);
    }

    int depth;
    int remainPiece = NUMGRIDS - board.score[BLACK] - board.score[WHITE];
    int score = 0;
    Board::Move move;
    int moveIdx = 0;
    for (depth = 0; depth < remainPiece &&
                    ((float) (clock() - startTime)) / CLOCKS_PER_SEC < timeLimit / 2.; depth++) {
        int alpha = INT_MIN, beta = INT_MAX;
        timeout = false;
        for (int i = 0; i < legalMove.size(); i++) {
            Board testBoard = board;
            testBoard.ApplyMove(legalMove[i]);
            score = alphabetaPruning(testBoard, depth, alpha, beta, false);
            if (timeout) {
                break;
            }
            if (score >= alpha) {
                move = legalMove[i];
                moveIdx = i;
                alpha = score;
            } // Not randomizing when score == alpha
        }
    }
    cout << "Depth: " << depth << " in " <<
         ((float) (clock() - startTime)) / CLOCKS_PER_SEC << " seconds" << endl;
    cout << "Choose move " << moveIdx << endl;
    board.PrintBoard(vector<Board::Move>(1, move), true, firstPlayer);
    board.ApplyMove(move);
    return board.SwitchPlayer(false);
}


void Game::Play() {
    bool gameOver = false;
    randomizer.seed(0);

    cout << "Game Start" << endl << endl;
    board.PrintBoard(vector<Board::Move>(), false, firstPlayer);

    while (!gameOver) {
        if (humanPlayer[board.currentPlayer]) {
            gameOver = HumanMove();
        } else {
            gameOver = RandomMove();
        }
    }
    board.PrintBoard(vector<Board::Move>(), false, firstPlayer);
    board.GameOver();
}

#endif //_GAME_CPP_