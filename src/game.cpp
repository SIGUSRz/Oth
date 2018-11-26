#ifndef _GAME_CPP_
#define _GAME_CPP_

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <climits>
#include <time.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include "game.h"
#include "board.h"
#include "const.h"

using namespace std;

Game::Game(){}

void Game::setup(int gameType, string board_file) {
    switch(gameType) {
        case(0):
            humanPlayer[BLACK] = true;
            humanPlayer[WHITE] = false;
            break;
        case(1):
            humanPlayer[BLACK] = false;
            humanPlayer[WHITE] = true;
        default:
            throw;
    }
    if (!board_file.empty()) {
        int state[BOARDSIZE][BOARDSIZE];
        int player;
        int i = 0, j = 0;
        fstream fs;

        fs.open(board_file.c_str(), fstream::in);
        char c;
        while((c = fs.get()) != EOF) {
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
        fs >> timeLimit;
        board = Board(state, player);
        fs.close();
    } else {
        string t;
        board = Board();
    }

}

#endif //_GAME_CPP_