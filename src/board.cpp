#ifndef _BOARD_CPP_
#define _BOARD_CPP_

#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include "const.h"
#include "board.h"

using namespace std;

Board::Board() {
    //initialize empty board
    for (int i = 0; i < BOARDSIZE; i++) {
        for (int j = 0; j < BOARDSIZE; j++) {
            board[i][j] = 0;
        }
    }

    //initialize starting pieces
    board[4][3] = BLACK;
    board[3][4] = BLACK;
    board[3][3] = WHITE;
    board[4][4] = WHITE;

    currentPlayer = BLACK;
    playerPassed = false;
    score[BLACK] = 2;
    score[WHITE] = 2;
}

//Board::Board(Board *board)
//  copy constructor
Board::Board(Board &b){
    for(int i = 0; i < BOARDSIZE; i++)
        for(int j = 0; j < BOARDSIZE; j++)
            this->board[i][j] = b.board[i][j];
    for(int i = 0; i < 3; i++)
        this->score[i] = b.score[i];

    this->currentPlayer = b.currentPlayer;
    this->playerPassed = b.playerPassed;
}


//Board::Board(char boardState[8][8], int currentPlayer)
//  constructor to initialize board with boardState and currentPlayer
Board::Board(int boardState[8][8], int currentPlayer){
    score[BLACK] = 0;
    score[WHITE] = 0;
    for(int i = 0; i < BOARDSIZE; i++){
        for(int j = 0; j < BOARDSIZE; j++){
            board[i][j] = boardState[i][j];
            if(boardState[i][j] == WHITE)
                score[WHITE]++;
            else if(boardState[i][j] == BLACK)
                score[BLACK]++;
        }
    }
    this->currentPlayer = currentPlayer;
    playerPassed = false;
}

#endif //_BOARD_CPP_