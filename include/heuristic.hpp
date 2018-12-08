//
// Created by ZengS on 2018/12/4.
//

#ifndef OTH_HEURISTIC_HPP
#define OTH_HEURISTIC_HPP

#include <cmath>
#include <numeric>
#include <iostream>
#include "board.hpp"

using namespace std;

class Heuristic {
public:
    int Minimax_Heuristic(Board &board);

private:
    int maxPlayer;
    int minPlayer;
    vector<bool> stableDiscs;

    int Utility(Board &board);

    int DiscScore(Board &board);

    int MobilityScore(Board &board);

    int NegColor(int color);

    int PotentialMobility(Board &board);

    int PlayerPotentialMobility(Board &board, int color);

    int CornerScore(Board &board);

    int CornerLossScore(Board &board);

    int SquareWeights(Board &board);

    int Parity(Board &board);

    int StabilityScore(Board &board);

    int StableCorner(Board &board, int color);

    int EdgeScore(Board &board);

    int FrontierScore(Board &board);
};

#endif //OTH_HEURISTIC_HPP
