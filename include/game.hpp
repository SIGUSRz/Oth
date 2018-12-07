//
// Created by ZengS on 2018/11/18.
//

#ifndef OTH_GAME_H
#define OTH_GAME_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <random>
#include <climits>
#include <ctime>
#include <cctype>
#include <iomanip>
#include <cstdlib>
#include "game.hpp"
#include "player.hpp"
#include "board.hpp"
#include "const.hpp"

using namespace std;

class Game{
public:
    void setup(int p1, int p2, float timeLimit, string file_list);
    void Play();
    void ApplyMove();
    void CheckGameOver();

private:

    Player blackPlayer;
    Player whitePlayer;
    bool gameOver;
    int currentColor;
    Board board;
    minstd_rand randomizer;
};

#endif //OTH_GAME_H
