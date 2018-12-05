//
// Created by ZengS on 2018/11/18.
//

#ifndef OTH_GAME_H
#define OTH_GAME_H

#include <string>
#include <ctime>
#include <random>
#include "board.hpp"
#include "const.hpp"
#include "heuristic.hpp"

using namespace std;

class Game{
public:
    void setup(int gameType, int timeLimit, string file_list);
    void Play();

private:
    int alphabetaPruning(Board board, int depth, int alpha, int beta, bool maxPlayer);
    bool AutoMove();
    bool HumanMove();
    bool RandomMove();

    Heuristic heuristic;
    int firstPlayer;
    Board board;
    int maxPlayer; //The Max of MiniMax
    bool humanPlayer[3];
    int timeLimit;
    clock_t startTime;
    bool timeout;
    minstd_rand randomizer;
};

#endif //OTH_GAME_H
