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
#include "game.hpp"
#include "board.hpp"
#include "const.hpp"

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

int Game::alphabetaPruning(Board board, int depth, int alpha, int beta, bool maxPlayer) {
    int a = alpha, b = beta;

    if ((float) (clock() - startTime) / CLOCKS_PER_SEC > STOPTIME * timeLimit) {
        timeout = true;
        return heuristic.Minimax_Heuristic(board, maxPlayer);
    } else if (depth == 0) {
        return heuristic.Minimax_Heuristic(board, maxPlayer);
    } else {
        depth--;
    }
    vector<Board::Move> m = board.LegalMoves(board.currentPlayer);

    if (m.empty()) {
        if (board.TerminalState(true)) {
            Board testBoard = board;
            testBoard.SwitchPlayer(false);
            return heuristic.Minimax_Heuristic(board, maxPlayer);
        } else {
            Board testBoard = board;
            testBoard.SwitchPlayer(true);
            return alphabetaPruning(testBoard, depth, alpha, beta, !maxPlayer);
        }
    }

    if (maxPlayer) {
        int value = INT_MIN;
//        double v = numeric_limits<double>::max();
        for (auto &move : m) {
            Board testBoard = board;
            board.ApplyMove(move);
            board.SwitchPlayer(false);
            int score = alphabetaPruning(testBoard, depth, alpha, beta, false);
            value = MAX(value, score);

            if (value >= b) {
                return value;
            }
            a = MAX(a, value);
        }
        return value;
    } else {
        int value = INT_MAX;
        for (auto &move : m) {
            Board testBoard = board;
            testBoard.ApplyMove(move);
            testBoard.SwitchPlayer(false);
            int score = alphabetaPruning(testBoard, depth, alpha, beta, true);
            value = MIN(value, score);

            if (value <= a) {
                return value;
            }
            b = MIN(b, value);
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
//    cout << "Depth: " << depth << " in " <<
//         ((float) (clock() - startTime)) / CLOCKS_PER_SEC << " seconds" << endl;
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
            gameOver = AutoMove();
        }
    }
    board.PrintBoard(vector<Board::Move>(), false, firstPlayer);
    board.GameOver();
}

#endif //_GAME_CPP_