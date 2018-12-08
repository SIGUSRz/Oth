#ifndef OTH_GAME_H
#define OTH_GAME_H

#include <iostream>
#include <string>
#include <vector>
#include <climits>
#include <iomanip>
#include "game.hpp"
#include "player.hpp"
#include "board.hpp"
#include "const.hpp"

using namespace std;

class Game {
public:
    void Setup(int p1, int p2, float timeLimit, string file_list);

    void Play();

    void ApplyMove();

    void CheckGameOver();

private:

    Player blackPlayer;
    Player whitePlayer;
    bool gameOver;
    int currentColor;
    Board board;
};

#endif //OTH_GAME_H
