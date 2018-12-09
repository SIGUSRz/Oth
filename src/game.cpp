#include "game.hpp"

using namespace std;

void Game::Setup(int p1, int p2, float timeLimit, string file_list) {
    if (!file_list.empty()) {
        cout << "File read not supported" << endl;
        this->board = Board();
        this->board.timeLimit = timeLimit;
        this->blackPlayer.isAI = (p1 != 1);
        this->whitePlayer.isAI = (p2 != 1);
        this->currentColor = BLACK;
    } else {
        this->board = Board();
        this->board.timeLimit = timeLimit;
        this->blackPlayer.isAI = (p1 != 1);
        this->whitePlayer.isAI = (p2 != 1);
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
        if (blackPlayer.isAI) {
            if (!blackPlayer.timeLog.empty()) {
                cout << "Black Average Search Time: "
                     << accumulate(blackPlayer.timeLog.begin(),
                                   blackPlayer.timeLog.end(), 0.0) / blackPlayer.timeLog.size()
                     << endl;
            }
        }
        if (whitePlayer.isAI) {
            if (!whitePlayer.timeLog.empty()) {
                cout << "White Average Search Time: "
                     << accumulate(whitePlayer.timeLog.begin(),
                                   whitePlayer.timeLog.end(), 0.0) / whitePlayer.timeLog.size()
                     << endl;
            }
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

    this->step = 0;
    while (!this->gameOver) {
        this->step += 1;
        ApplyMove();
        CheckGameOver();
    }
}