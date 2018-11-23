#include <iostream>
#include <fstream>
#include <vector>
#include <climits>
#include <time.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include "include/game.h"
#include "include/board.h"
#include "include/const.h"

using namespace std;

Game::Game() {}

void Game::setup(int gameType) {
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

}