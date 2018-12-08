//
// Created by ZengS on 2018/12/4.
//

#ifndef OTH_PLAYER_HPP
#define OTH_PLAYER_HPP

#include <vector>
#include <string>
#include <iostream>
#include <chrono>
#include <climits>
#include <random>
#include <array>
#include "const.hpp"
#include "heuristic.hpp"
#include "board.hpp"

class Player {
public:
    int color;
    bool isAI;

    Board::Move FindMove(Board board, bool &pass);

private:
    Heuristic heuristic;
    int maxPlayer;
    minstd_rand randomizer;

    std::chrono::time_point<std::chrono::system_clock> startTimer();

    float stopTimer(std::chrono::time_point<std::chrono::system_clock> startTime);

    Board::Move HumanMove(Board board, bool &pass);

    Board::Move AIMove(Board board, bool &pass);

    int AlphaBetaPruning(Board board, int depth,
                         std::chrono::time_point<std::chrono::system_clock>,
                         int alpha, int beta, bool maxPlayer);
};

#endif //OTH_PLAYER_HPP
