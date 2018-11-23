//
// Created by ZengS on 2018/11/18.
//

#ifndef OTH_GAME_H
#define OTH_GAME_H

#include <time.h>
#include "board.h"
#include "const.h"

using namespace std;

class Game{
public:
    Game();
    void setup(int gameType);
    void play();

private:
    int heuristic(Board board);
    int alphabeta(Board board, int depth, int alpha, int beta, bool maxPlayer);
    bool smartMove();
    bool humanMove();
    bool randomMove();

    Board board;
    int maxPlayer;
    bool humanPlayer[3];
    int timeLimit;
    clock_t startTime;
    bool timeout;
};

#endif //OTH_GAME_H
