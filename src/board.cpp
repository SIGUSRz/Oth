#ifndef _BOARD_CPP_
#define _BOARD_CPP_

#include <iostream>
#include <iomanip>
#include <vector>
#include "const.hpp"
#include "board.hpp"

using namespace std;

const int directions[4] = {1, 2, 3, 4};
const int steps[2] = {-1, 1};

Board::Grid::Grid(int y, int x) {
    this->y = y;
    this->x = x;
}

Board::Move::Move(int y, int x) {
    this->grid.y = y;
    this->grid.x = x;
    this->valid = false;
}

//constructor
Board::Board() {
    //empty board
    for (int i = 0; i < BOARDSIZE; i++) {
        vector<int> tmp;
        for (int j = 0; j < BOARDSIZE; j++) {
            tmp.push_back(0);
        }
        board.push_back(tmp);
    }

    //starting pieces
    board[4][3] = BLACK;
    board[3][4] = BLACK;
    board[3][3] = WHITE;
    board[4][4] = WHITE;

    currentPlayer = BLACK;
    playerPassed = false;
    score[BLACK] = 2;
    score[WHITE] = 2;
}


// constructor with board
Board::Board(const Board &b) {
    for (int i = 0; i < BOARDSIZE; i++) {
        vector<int> tmp;
        for (int j = 0; j < BOARDSIZE; j++) {
            tmp.push_back(b.board[i][j]);
        }
        board.push_back(tmp);
    }
    for (int i = 0; i < 3; i++) {
        score[i] = b.score[i];
    }

    currentPlayer = b.currentPlayer;
    playerPassed = b.playerPassed;
}

//  constructor with exist board and player
Board::Board(int boardState[8][8], int currentPlayer) {
    score[BLACK] = 0;
    score[WHITE] = 0;
    for (int i = 0; i < BOARDSIZE; i++) {
        for (int j = 0; j < BOARDSIZE; j++) {
            board[i][j] = boardState[i][j];
            if (boardState[i][j] == WHITE) {
                score[WHITE]++;
            } else if (boardState[i][j] == BLACK) {
                score[BLACK]++;
            }
        }
    }
    this->currentPlayer = currentPlayer;
    playerPassed = false;
}

// print board: player move in yellow, computer move in green
void Board::PrintBoard(vector<Board::Move> moves, bool player, int firstPlayer) {
    if (firstPlayer == 0) {
        cout << "Your Piece: " << RED << setw(2) << BLACK << RESET << endl;
        cout << "Computer Piece: " << BLUE << setw(2) << WHITE << RESET << endl;
    } else {
        cout << "Your Piece: " << BLUE << setw(2) << WHITE << RESET << endl;
        cout << "Computer Piece: " << RED << setw(2) << BLACK << RESET << endl;
    }
    cout << "Your Valid Move: " << GREEN << setw(2) << 0 << RESET << endl;
    cout << "Computer Valid Move: " << YELLOW << setw(2) << 0 << RESET << endl;
    cout << "    0  1  2  3  4  5  6  7" << endl;
    cout << "   ------------------------" << endl;
    for (int i = 0; i < BOARDSIZE; i++) {
        cout << i << " |";
        for (int j = 0; j < BOARDSIZE; j++) {
            bool potentialMove = false;
            for (int k = 0; k < moves.size(); k++) {
                if (moves[k].grid.y == i && moves[k].grid.x == j) {
                    if (!player) {
                        cout << GREEN << setw(2) << board[i][j] << RESET << " ";
                    } else {
                        cout << YELLOW << setw(2) << k << RESET << " ";
                    }
                    potentialMove = true;
                }
            }
            if (!potentialMove) {
                if (board[i][j] == WHITE) {
                    cout << BLUE;
                } else if (board[i][j] == BLACK) {
                    cout << RED;
                }
                cout << setw(2) << board[i][j] << RESET << " ";
            }
        }
        cout << "|" << endl;
    }
    cout << "   ------------------------" << endl;
}

// check if the grid is on frontier,
// where it's the piece between other piece and blank space
bool Board::OnFrontier(int y, int x) {
    if (board[y][x] == 0) {
        return false;
    }
    for (auto &direction : directions) {
        for (auto &step : steps) {
            int Y = y, X = x;
            MoveAlong(Y, X, direction, step);
            if (OnBoard(Y, X)) {
                if (board[Y][X] != 0) {
                    return true;
                }
            }
        }
    }
    return false;
}


bool Board::MoveAlong(int &y, int &x, int direction, int step) {
    if ((step != 1) && (step != -1)) {
        return false;
    }

    switch (direction) {
//      Horizontal
        case (1):
            x += step;
            return true;
//      Vertical
        case (2):
            y += step;
            return true;
//      decrease diagonal
        case (3):
            x += step;
            y += step;
            return true;
//      increase diagonal
        case (4):
            x += step;
            y -= step;
            return true;
        default:
            cout << "wrong direction = " << direction << endl;
            throw;
    }
}

bool Board::OnBoard(int y, int x) {
    return (x >= 0) && (x < BOARDSIZE) && (y >= 0) && (y < BOARDSIZE);
}

bool Board::TerminalState(bool currentPlayerPass) {
    return (playerPassed && currentPlayerPass) || (score[BLACK] + score[WHITE] == NUMGRIDS);
}

bool Board::SwitchPlayer(bool currentPlayerPass) {
    if (this->TerminalState(currentPlayerPass)) {
        return true;
    }
    currentPlayer = (currentPlayer == WHITE) ? BLACK : WHITE;
    playerPassed = false;
    return false;
}

void Board::ApplyMove(Board::Move move) {
    board[move.grid.y][move.grid.x] = currentPlayer;
    if (currentPlayer == WHITE) {
        score[WHITE]++;
    } else {
        score[BLACK]++;
    }
//    cout << "Flips: " << endl;
    for (auto &flip : move.flips) {
//        cout << flip.y << flip.x << endl;
        board[flip.y][flip.x] = currentPlayer;
        if (currentPlayer == WHITE) {
            score[WHITE]++;
            score[BLACK]--;
        } else {
            score[BLACK]++;
            score[WHITE]--;
        }
    }
}

vector<Board::Move> Board::LegalMoves(int player) {
    vector<Board::Move> moves;

    for (int i = 0; i < BOARDSIZE; i++) {
        for (int j = 0; j < BOARDSIZE; j++) {
            if (board[i][j] != 0) {
                continue;
            }

            Board::Move move = Board::Move(i, j);

            for (auto &direction : directions) {
                for (auto &step : steps) {
                    int y = move.grid.y, x = move.grid.x;
                    vector<Board::Grid> trace;
                    this->MoveAlong(y, x, direction, step);
                    //not a valid direction unless opponent's piece is next
                    if (OnBoard(y, x)) {
                        if ((board[y][x] == player) || (board[y][x] == 0)) {
                            continue;
                        }
                    }
                    while (OnBoard(y, x)) {
                        if (board[y][x] == player) {
                            //mark move as valid and append trace to flips vector
                            move.valid = true;
                            move.flips.insert(move.flips.end(), trace.begin(), trace.end());
                            break;
                        } else if (board[y][x] == 0) {
                            break;
                        } else {
                            trace.emplace_back(Board::Grid(y, x)); //keep track of potential flips
                        }
                        this->MoveAlong(y, x, direction, step);
                    }
                }
            }
            if (move.valid)
                moves.push_back(move);
        }
    }
    return moves;
}

void Board::GameOver() {
    cout << "GAME OVER" << endl;
    cout << "white: " << score[WHITE] << " black : " << score[BLACK] << endl;
    if(score[WHITE] > score[BLACK]) {
        cout << "White wins!" << endl;
    } else if(score[WHITE] < score[BLACK]) {
        cout << "Black wins!" << endl;
    } else {
        cout << "Tie!" << endl;
    }
}
#endif //_BOARD_CPP_