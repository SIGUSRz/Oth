//
// Created by ZengS on 2018/11/18.
//

#ifndef OTH_GAME_H
#define OTH_GAME_H

#include <string>
#include <ctime>
#include <random>
#include "board.h"
#include "const.h"

using namespace std;

class Game{
public:
    Game() = default;
    void setup(int gameType, int timeLimit, string file_list);
    void Play();

private:
//    int heuristic(Board board);
//    int alphabeta(Board board, int depth, int alpha, int beta, bool maxPlayer);
//    bool smartMove();
    bool HumanMove();
    bool RandomMove();

    int firstPlayer;
    Board *board;
    int maxPlayer;
    bool humanPlayer[3];
    int timeLimit;
    clock_t startTime;
    bool timeout;
    minstd_rand randomizer;
};

#endif //OTH_GAME_H
