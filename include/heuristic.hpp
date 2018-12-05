//
// Created by ZengS on 2018/12/4.
//

#ifndef OTH_HEURISTIC_HPP
#define OTH_HEURISTIC_HPP

#include <cmath>
#include "board.hpp"
using namespace std;

class Heuristic {
public:
    int Minimax_Heuristic(Board board, int maxPlayer);
};
#endif //OTH_HEURISTIC_HPP
