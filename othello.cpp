#include <iostream>
#include <string>
#include <boost/program_options.hpp>
#include "include/game.h"
using namespace std;

int main(int args, char *argv[]) {
    int gameType; // 0: player first, 1: opponent first
    Game game;
    game.setup(0);
    return 0;
}
