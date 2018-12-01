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
    public:
        Grid() = default;

        Grid(int y, int x);

        int x, y;
    };

    class Move {
    public:
        Move() = default;

        Move(int y, int x);

        Board::Grid grid;
        bool valid = false;
        vector<Grid> flips;
    };

    Board();

    Board(Board &b);

    Board(int board[8][8], int current_player);

    int currentPlayer;
    int score[3];
    int board[BOARDSIZE][BOARDSIZE];
    bool playerPassed;
    const int directions[4] = {1, 2, 3, 4};
    const int steps[2] = {-1, 1};

    void PrintBoard(vector<Board::Move> moves, bool player, int firstPlayer);

    bool OnFrontier(int y, int x);

    bool TerminalState(bool currentPlayerPass);

    bool SwitchPlayer(bool currentPlayerPass);

    void ApplyMove(Board::Move move);

    vector<Board::Move> LegalMoves(int player);

    void GameOver();

private:
    bool OnBoard(int y, int x);

    bool MoveAlong(int &y, int &x, int direction, int step);
};

#endif //OTH_BOARD_H