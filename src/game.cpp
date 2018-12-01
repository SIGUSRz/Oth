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
        board = new Board(state, player);
        timeLimit = tLimit;
        fs.close();
    } else {
        timeLimit = tLimit;
        board = new Board();
    }
}

bool Game::RandomMove() {
    vector<Board::Move> m = board->LegalMoves(board->currentPlayer);
    if (!m.empty()) {
        Board::Move randMove = m[randomizer() % m.size()];
        board->PrintBoard(vector<Board::Move>(1, randMove), true, firstPlayer);
        board->ApplyMove(randMove);
        return board->SwitchPlayer(false);
    } else {
        cout << "Computer had to pass :(" << endl;
        return board->SwitchPlayer(true);
    }
}

//bool Game::HumanMove()
//  method to apply move to game based on human input
//  returns false if game in terminal state
bool Game::HumanMove() {
    int moveNum = -1;
    string in;
    vector<Board::Move> m = board->LegalMoves(board->currentPlayer);
    if (!m.empty()) {
        board->PrintBoard(m, false, firstPlayer);
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
        board->ApplyMove(m[moveNum]);
        return board->SwitchPlayer(false);
    } else {
        cout << "Sorry, you have to pass :(" << endl;
        return board->SwitchPlayer(true);
    }
}

void Game::Play() {
    bool gameOver = false;
    randomizer.seed(0);

    cout << "Game Start" << endl << endl;
    board->PrintBoard(vector<Board::Move>(), false, firstPlayer);

    while (!gameOver) {
        if (humanPlayer[board->currentPlayer]) {
            gameOver = HumanMove();
        } else {
            gameOver = RandomMove();
        }
    }
    board->PrintBoard(vector<Board::Move>(), false, firstPlayer);
    board->GameOver();
}

#endif //_GAME_CPP_