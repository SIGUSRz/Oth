//
// Created by ZengS on 2018/12/4.
//

#ifndef OTH_HEURISTIC_HPP
#define OTH_HEURISTIC_HPP

#include <cmath>
#include <numeric>
#include "board.hpp"

using namespace std;

class Heuristic {
public:
    int Minimax_Heuristic(Board board, int maxp, bool terminal);

private:
    int maxPlayer;
    int minPlayer;
    int discOnBoard;

    int Utility(Board board);

    int DiscScore(Board board);

    int MobilityScore(Board board);

    int NegColor(int color);

    int PotentialMobility(Board board);

    int PlayerPotentialMobility(Board board, int color);

    int CornerScore(Board board);

    int CornerLossScore(Board board);

    int SquareWeights(Board board);

    int Parity(Board board);

    void EdgeFrontierScore(Board board, vector<int> &res);
};

#endif //OTH_HEURISTIC_HPP
