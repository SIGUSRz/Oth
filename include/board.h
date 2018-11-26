//
// Created by ZengS on 2018/11/18.
//

#ifndef OTH_BOARD_H
#define OTH_BOARD_H

#include <vector>
#include "const.h"

using namespace std;

class Board {
public:
    class Grid {
        Grid();
        Grid(int x, int y);
        int x, y;
    };

    class Move {
        Move();
        Move(int x, int y);
        Board::Grid grid;
        bool valid;
        vector<Grid> flips;
    };

    Board();
    Board(Board &b);
    Board(int board[8][8], int current_player);

    int currentPlayer;
    int score[3];
    int board[BOARDSIZE][BOARDSIZE];
    bool playerPassed;

};

#endif //OTH_BOARD_H