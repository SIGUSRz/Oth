#ifndef _GAME_CPP_
#define _GAME_CPP_

#include "game.hpp"

using namespace std;

void Game::setup(int blackIsHuman, int whiteIsHuman, float tLimit, string board_file) {
    if (!board_file.empty()) {
        int state[BOARDSIZE][BOARDSIZE];
        int player;
        int i = 0, j = 0;
        fstream fs;

        fs.open(board_file.c_str(), fstream::in);
        char c;
        while ((c = fs.get()) != EOF) {
            if (c == '0' || c == '1' || c == '2') {
                state[i][j++] = c - '0';
                if (j == BOARDSIZE) {
                    j = 0;
                    ++i;
                    if (i == BOARDSIZE) {
                        break;
                    }
                }
            }
        }

        fs >> player;
        board = Board(state, player);
        board.timeLimit = tLimit;
        fs.close();
    } else {
        this->board = Board();
        this->board.timeLimit = tLimit;
        this->blackPlayer.color = BLACK;
        this->whitePlayer.color = WHITE;
        this->blackPlayer.isAI = (blackIsHuman != 1);
        this->whitePlayer.isAI = (whiteIsHuman != 1);
        this->currentColor = BLACK;
    }
}

void Game::ApplyMove() {
    Board::Move move;

    if (this->currentColor == BLACK) {
        cout << "Black " << RED << setw(1) << BLACK << RESET << " to move" << endl;
        this->board.currentPlayer = BLACK;
        move = this->blackPlayer.FindMove(this->board, this->board.pass[0]);
    } else if (this->currentColor == WHITE) {
        cout << "White " << BLUE << setw(2) << WHITE << RESET << " to move" << endl;
        this->board.currentPlayer = WHITE;
        move = this->whitePlayer.FindMove(this->board, this->board.pass[0]);
    }
    if (!this->board.pass[0]) {
        this->board.UpdateBoard(move);
    } else {
        this->board.SwitchPlayer();
    }
    this->currentColor = this->board.currentPlayer;
    this->board.PrintBoard(this->board.FindLegalMoves(this->currentColor));
}

void Game::CheckGameOver() {
    if (this->board.pass[0] && this->board.pass[1]) {
        cout << "GAME OVER" << endl;
        cout << "white: " << this->board.whiteScore <<
             " black : " << this->board.blackScore << endl;
        if (this->board.whiteScore > this->board.blackScore) {
            cout << "White wins!" << endl;
        } else if (this->board.whiteScore < this->board.blackScore) {
            cout << "Black wins!" << endl;
        } else {
            cout << "Tie!" << endl;
        }
        this->gameOver = true;
    } else {
        this->board.pass[1] = this->board.pass[0];
        this->board.pass[0] = false;
    }
}


void Game::Play() {
    this->gameOver = false;

    cout << "Game Start" << endl << endl;
    this->board.PrintBoard(this->board.FindLegalMoves(this->currentColor));

    while (!this->gameOver) {
        ApplyMove();
        CheckGameOver();
    }
}

#endif //_GAME_CPP_